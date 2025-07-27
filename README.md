# HueGen - Intelligent Color Palette Extractor

HueGen is a sophisticated C++ application that extracts dominant color palettes from images using advanced computer vision techniques and generates customizable theme files for various applications.

## Features

- **Intelligent Color Extraction**: Uses K-means clustering in LAB color space for perceptually accurate color grouping
- **Smart Color Selection**: Implements a greedy algorithm to select the most visually distinct colors
- **Multiple Color Formats**: Supports HEX, RGB, RGBA, HSL, HSV, and LAB color representations
- **Template Engine**: Flexible template system for generating theme files
- **Saturation-Based Filtering**: Prioritizes vibrant colors while filtering out low-saturation tones
- **Cross-Platform**: Built with OpenCV and standard C++ libraries

## Prerequisites

### Required Dependencies

- **OpenCV 4.x** - Computer vision and image processing
- **nlohmann/json** - JSON parsing and generation
- **C++17 compiler** (GCC 7+, Clang 5+, or MSVC 2017+)
- **CMake 3.10+** (recommended for building)

### Installation on Linux

```
./install.sh
```

## Building

### Using Make (Simple)

```bash
g++ -std=c++17 -o huegen *.cpp -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
```

### Using CMake (Recommended)

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

### Basic Usage

```bash
./huegen <image_path>
```

### Example

```bash
./huegen ~/Pictures/sunset.jpg
```

The program will:

1. Load and resize the input image to 200x200 pixels
2. Extract up to 32 color clusters using K-means
3. Filter out colors with low lightness (L < 30 in LAB space)
4. Select the 16 most distinct colors using a greedy selection algorithm
5. Sort colors by saturation (most vibrant first)
6. Generate theme files from templates

## Output

### Generated Files

Theme files are automatically generated in:

```
~/.config/huegen/themes/
```

### Color Data Structure

Each extracted color includes:

- **HEX**: `#FF6B35`
- **RGB**: `rgb(255, 107, 53)`
- **RGBA**: `rgba(255, 107, 53, 1.0)`
- **HSL**: `hsl(16, 100%, 60%)`
- **HSV**: `hsv(16, 79%, 100%)`
- **LAB**: `lab(69, 42, 56)`

## Template System

### Template Files (.tlp)

Create template files in the `../templates/` directory with `.tlp` extension.

### Template Syntax

Use placeholder syntax: `{colorN.property}`

#### Example Template (theme.tlp)

```css
/* Generated Theme */
:root {
  --primary: {color0.hex};
  --secondary: {color1.hex};
  --accent: {color2.hex};
  --background: {color15.hex};
}

.primary-color {
  background-color: {color0.hex};
  color: {color0.rgb};
}

.gradient {
  background: linear-gradient(45deg, {color0.hex}, {color1.hex});
}
```

#### Available Properties

- `hex` - Hexadecimal format (#FF6B35)
- `rgb` - RGB function format
- `rgba` - RGBA function format
- `hsl` - HSL function format
- `hsv` - HSV function format
- `lab` - LAB function format

#### Color Indices

- `color0` to `color15` - Colors sorted by saturation (most vibrant first)
- `color0` - Most saturated/vibrant color
- `color15` - Least saturated color

## Algorithm Details

### Color Extraction Process

1. **Image Preprocessing**

   - Resize to 200x200 for consistent processing
   - Convert to LAB color space for perceptual uniformity

2. **K-means Clustering**

   - Extract 32 initial color clusters
   - Use PP_CENTERS initialization for better cluster distribution

3. **Color Filtering**

   - Remove colors with lightness < 30 (too dark)
   - Quantize colors to reduce noise

4. **Distinct Color Selection**

   - Start with highest saturation color
   - Greedily select colors that maximize minimum distance to selected colors
   - Uses Euclidean distance in LAB space

5. **Final Sorting**
   - Sort by saturation for consistent color ordering

### Key Functions

- `extractClusterColors()` - K-means clustering and color extraction
- `selectMostDistinctColors()` - Greedy selection algorithm
- `calculateSaturation()` - Saturation calculation in LAB space
- `colorDistance()` - Euclidean distance between colors
- `processTemplates()` - Template processing engine

## Project Structure

```
huegen/
├── color_selector.hpp/cpp     # Color selection algorithms
├── color_utils.hpp/cpp        # Color space conversions
├── kmeans_wrapper.hpp/cpp     # K-means clustering wrapper
├── template_engine.hpp/cpp    # Template processing
├── main.cpp                   # Main application
└── templates/                 # Template files (.tlp)
    ├── vim.tlp
    ├── terminal.tlp
    └── css.tlp
```

## Advanced Usage

### Custom Template Directory

Modify the `templateDir` variable in main.cpp:

```cpp
std::string templateDir = "/path/to/your/templates";
```

### Custom Output Directory

Modify the `outputDir` variable in main.cpp:

```cpp
std::string outputDir = "/path/to/output/";
```

### Adjusting Color Count

Change the number of extracted colors by modifying:

```cpp
auto distinctColors = selectMostDistinctColors(filtered, 16); // Change 16 to desired count
```

## Examples

### Terminal Theme Generation

Create `terminal.tlp`:

```bash
# Terminal Color Scheme
foreground={color0.hex}
background={color15.hex}
color0={color8.hex}
color1={color1.hex}
color2={color2.hex}
# ... continue for all 16 colors
```

### Vim Theme Generation

Create `vim.tlp`:

```vim
" Generated Vim Colorscheme
hi Normal guifg={color0.hex} guibg={color15.hex}
hi Comment guifg={color8.hex}
hi String guifg={color2.hex}
hi Function guifg={color4.hex}
```

## Troubleshooting

### Common Issues

1. **OpenCV not found**

   ```bash
   export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
   ```

2. **Permission denied for output directory**

   ```bash
   mkdir -p ~/.config/huegen/themes/
   chmod 755 ~/.config/huegen/themes/
   ```

3. **Template not processing**
   - Ensure template files have `.tlp` extension
   - Check template syntax: `{colorN.property}`
   - Verify template directory path

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is open source. Please check the repository for license details.

## Acknowledgments

- Built with OpenCV for robust image processing
- Uses LAB color space for perceptually uniform color analysis
- Inspired by modern color palette extraction techniques
