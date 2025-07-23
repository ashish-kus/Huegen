# Huegen 🎨

Fast C++ wallpaper color extractor. Generates 16 colors (8 base + 8 bright) from any image for terminal theming.

![Huegen Preview](./Previews/preview.gif)

## Quick Start

```bash
# Install dependencies
sudo apt install libopencv-dev

# Compile
g++ main.cpp -o huegen -I/usr/include/opencv4 -lopencv_core -lopencv_imgcodecs -lopencv_imgproc


# Extract colors
./huegen wallpaper.jpg > theme.css
```

## Output

```css
* {
  --color0: #B25426;
  --color1: #3683D9;
  ...
  --color15: #25593E;
}
```

## Why Huegen?

- **Fast**: C++ + OpenCV for speed
- **Simple**: Single binary, minimal deps
- **Smart**: K-means clustering with distance filtering

## Integration

```bash
# Alacritty
./extractor img.jpg | sed 's/--color/color_/g' >> ~/.config/alacritty/alacritty.yml

# Any theme system
./extractor img.jpg > colors.css
```

MIT License • PRs welcome

# Huegen

#### Project Architecture

```
color_extractor/
│
├── include/
│   ├── color_utils.hpp        // Color conversion, distance, etc.
│   ├── kmeans_wrapper.hpp     // Encapsulate OpenCV k-means logic
│   ├── css_output.hpp         // Handles generating CSS/other formats
│   └── config.hpp             // Constants and thresholds
│
├── src/
│   ├── main.cpp               // CLI interface
│   ├── color_utils.cpp        // Implementation of color_utils.hpp
│   ├── kmeans_wrapper.cpp     // Implementation of kmeans_wrapper.hpp
│   ├── css_output.cpp         // Implementation of css_output.hpp
│
├── templates/
│   └── rofi.template          // Example user-defined template
│
├── wallpapers/
│   └── image.jpg              // Example wallpapers
│
├── themes/
│   └── generated-theme.css    // Generated output
│
├── build/                     // Build artifacts
│
├── README.md
├── Makefile or CMakeLists.txt
└── LICENSE

```
