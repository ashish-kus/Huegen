# HeuGen

A high-performance C++ color palette generator that extracts vibrant and distinct colors from images, designed as a pywal alternative with superior color selection algorithms.

![Heugen Preview](./Previews/preview.gif)

## Overview

HeuGen (Heuristic Color Generator) is a command-line tool that analyzes images and generates aesthetically pleasing color palettes suitable for theming applications, wallpapers, and UI customization. Unlike traditional palette generators, HeuGen uses advanced clustering and color theory principles to ensure maximum color distinctiveness and visual appeal.

## Features

- **Intelligent Color Extraction**: Uses K-means clustering in LAB color space for perceptually accurate color analysis
- **Vibrant Color Selection**: Prioritizes saturated, visually appealing colors while filtering out overly dark tones
- **Distinct Color Algorithm**: Employs greedy selection to maximize color distinctiveness in the final palette
- **Dual Palette Generation**: Produces both vibrant colors and darker variants for comprehensive theming
- **High Performance**: Written in C++ with OpenCV for optimal speed and efficiency
- **Pywal-Compatible Output**: Generates hex color codes suitable for integration with existing theming workflows

## Algorithm Highlights

- **LAB Color Space Processing**: Works in perceptually uniform LAB color space for better color distance calculations
- **Smart Clustering**: Initial K-means clustering with 32 clusters to capture color diversity
- **Saturation-Based Prioritization**: Selects most saturated colors as starting points for palette generation
- **Greedy Distinctiveness**: Maximizes minimum distance between selected colors for optimal visual separation
- **Adaptive Darkening**: Creates darker variants with carefully calculated lightness reduction

## Requirements

- **OpenCV 4.x**: Computer vision library for image processing and color space conversions
- **C++11 or later**: Modern C++ compiler support
- **CMake 3.x** (recommended): For building the project

### Installing Dependencies

#### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install libopencv-dev cmake build-essential
```

#### macOS (with Homebrew)

```bash
brew install opencv cmake
```

#### Arch Linux

```bash
sudo pacman -S opencv cmake gcc
```

## Building

### Using Make

```bash
g++ -std=c++11 -o heugen heugen.cpp `pkg-config --cflags --libs opencv4`
```

### Using CMake (Recommended)

Create a `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.10)
project(heugen)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)

add_executable(heugen heugen.cpp)
target_link_libraries(heugen ${OpenCV_LIBS})
```

Then build:

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./heugen <image_path>
```

### Example

```bash
./heugen wallpaper.jpg
```

### Sample Output

```
# Vibrant Colors (8)
#FF6B35
#F7931E
#FFD23F
#06FFA5
#118AB2
#073B4C
#A663CC
#E63946

# Darker Variants (6)
#B24A26
#AD681A
#B2942C
#048066
#0C607D
#052934
```

## Output Format

HeuGen generates 14 colors total:

- **8 Vibrant Colors**: Highly saturated, distinct colors sorted by saturation
- **6 Darker Variants**: Progressively darker versions of the most vibrant colors

All colors are output as uppercase hex codes (e.g., `#FF6B35`) compatible with most theming applications.

## Integration Examples

### With Pywal

```bash
# Generate colors and save to file
./heugen image.jpg > colors.txt

# Use with your theming script
cat colors.txt | head -8 | tail -n +2 > ~/.cache/wal/colors
```

### Shell Script Integration

```bash
#!/bin/bash
COLORS=($(./heugen "$1" | grep -E '^#[0-9A-F]{6}$'))
export PRIMARY_COLOR=${COLORS[0]}
export SECONDARY_COLOR=${COLORS[1]}
# ... use colors for theming
```

## Technical Details

### Color Space Processing

- Converts images to LAB color space for perceptually uniform color analysis
- Resizes input images to 400x400 pixels for consistent processing speed
- Uses quantization to group similar colors and reduce noise

### Clustering Algorithm

- K-means clustering with 32 initial clusters
- Plusplus initialization for better cluster distribution
- Finds most frequent color in each cluster as representative

### Color Selection Logic

1. **Filtering**: Removes colors with lightness below threshold (L < 30)
2. **Distinctiveness**: Greedy algorithm maximizes minimum distance between colors
3. **Saturation Priority**: Starts selection with highest saturation colors
4. **Progressive Darkening**: Creates darker variants with factors: 0.7, 0.6, 0.5, 0.4, 0.3, 0.25

## Performance

- **Processing Time**: ~100-500ms for typical images (depending on complexity)
- **Memory Usage**: ~50-100MB peak memory consumption
- **Scalability**: Consistent performance regardless of input image size (due to resize preprocessing)

## Contributing

Contributions are welcome! Areas for improvement:

- Additional color space support (HSV, XYZ)
- Configurable palette sizes
- Alternative clustering algorithms
- Color harmony validation
- JSON/XML output formats

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by pywal and similar palette generation tools
- Built with OpenCV computer vision library
- Color theory based on CIE LAB color space research

## Troubleshooting

### Common Issues

**OpenCV not found**: Ensure OpenCV is properly installed and `pkg-config` can locate it

```bash
pkg-config --modversion opencv4
```

**Compilation errors**: Verify C++11 compiler support

```bash
g++ --version
```

**Runtime errors**: Check image file format support (JPEG, PNG, BMP, TIFF supported)
