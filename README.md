# Huegen - Color Theme Generator

Huegen is a C++ tool that extracts distinctive colors from images and generates theme files for various applications. It uses advanced color analysis techniques including K-means clustering and perceptual color distance calculations to create visually appealing color palettes.

🚧 Work in Progress:
The project has a strong and usable foundation, making it practical for day-to-day use. However, it still lacks many planned features. Contributions and improvements are welcome as development continues.

## Features

- **Intelligent Color Extraction**: Uses K-means clustering to identify dominant colors
- **Perceptual Color Selection**: Selects the most visually distinct colors using LAB color space
- **Multiple Color Formats**: Supports HEX, RGB, RGBA, HSL, HSV, and LAB formats
- **Template Engine**: Processes template files to generate configuration files for various applications
- **Automatic Filtering**: Removes colors that are too dark (lightness < 30)

## Installation

### Dependencies

- OpenCV 4.x
- nlohmann/json library
- C++17 compatible compiler
- CMake (recommended for building)

### Building

```bash
# Clone the repository
git clone <repository-url>
cd huegen

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Install (optional)
sudo make install
```

## Usage

A C++ tool that extracts visually distinct color palettes from images

```

Usage:
  huegen [OPTION...]

  -h, --help          Prints help.
  -i, --image arg     Path to the image file
  -o, --output arg    Output folder path (default:
                      /home/ashishk/.config/huegen/themes/)
  -t, --template arg  Template folder path (default:
                      /home/ashishk/.config/huegen/templates/)
  -v, --verbose       Enable verbose logging

```

### Basic Usage

```bash
./huegen -i <image_path>
```

**Example:**

```bash
./huegen -i ~/Pictures/wallpaper.jpg
```

### Directory Structure

Huegen expects the following directory structure in your home directory:

```
~/.config/huegen/
├── templates/          # Template files (.tlp extension)
│   ├── waybar.tlp
│   ├── hyprland.tlp
│   └── alacritty.tlp
└── themes/            # Generated theme files (created automatically)
    ├── waybar
    ├── hyprland
    └── alacritty
```

## Template System

### Template Files (.tlp)

Template files use a simple placeholder syntax to define where colors should be inserted:

```
{colorN.property}
```

Where:

- `N` is the color index (0-15)
- `property` is the color format (hex, rgb, hsl, etc.)

### Available Properties

For each color (color0 through color15), the following properties are available:

| Property | Example Output           | Description                      |
| -------- | ------------------------ | -------------------------------- |
| `hex`    | `#FF5733`                | Hexadecimal color code with #    |
| `strip`  | `FF5733`                 | Hexadecimal color code without # |
| `rgb`    | `rgb(255, 87, 51)`       | RGB color function               |
| `rgba`   | `rgba(255, 87, 51, 1.0)` | RGBA color function              |
| `hsl`    | `hsl(12, 100%, 60%)`     | HSL color function               |
| `hsv`    | `hsv(12, 80%, 100%)`     | HSV color function               |
| `lab`    | `lab(65, 45, 35)`        | LAB color function               |

### Example Templates

#### Waybar Template (`waybar.tlp`)

```css
* {
    border: none;
    border-radius: 0;
    font-family: "JetBrains Mono";
    font-size: 13px;
    min-height: 0;
}

window#waybar {
    background-color: {color0.hex};
    border-bottom: 3px solid {color1.hex};
    color: {color15.hex};
}

#workspaces button {
    padding: 0 5px;
    background-color: transparent;
    color: {color8.hex};
}

#workspaces button:hover {
    background: {color2.rgba};
}

#workspaces button.focused {
    background-color: {color3.hex};
    color: {color0.hex};
}
```

#### Hyprland Template (`hyprland.tlp`)

```ini
general {
    col.active_border = {color4.strip} {color5.strip} 45deg
    col.inactive_border = {color8.strip}
}

decoration {
    col.shadow = {color0.strip}
}

misc {
    background_color = {color0.strip}
}
```

#### Alacritty Template (`alacritty.tlp`)

```yaml
colors:
  primary:
    background: "{color0.hex}"
    foreground: "{color15.hex}"

  normal:
    black: "{color0.hex}"
    red: "{color1.hex}"
    green: "{color2.hex}"
    yellow: "{color3.hex}"
    blue: "{color4.hex}"
    magenta: "{color5.hex}"
    cyan: "{color6.hex}"
    white: "{color7.hex}"

  bright:
    black: "{color8.hex}"
    red: "{color9.hex}"
    green: "{color10.hex}"
    yellow: "{color11.hex}"
    blue: "{color12.hex}"
    magenta: "{color13.hex}"
    cyan: "{color14.hex}"
    white: "{color15.hex}"
```

## Configuration

### Environment Variables

The tool uses the following paths:

- Template directory: `$HOME/.config/huegen/templates/`
- Output directory: `$HOME/.config/huegen/themes/`

### Custom Templates

To create custom templates:

1. Create a new `.tlp` file in the templates directory
2. Use the `{colorN.property}` syntax for color placeholders
3. Run huegen to generate the themed file

## Integration Examples

### Waybar Integration

```bash
# Generate themes
./huegen ~/Pictures/current_wallpaper.jpg

# Copy generated config
cp ~/.config/huegen/themes/waybar ~/.config/waybar/style.css

# Reload waybar
pkill -SIGUSR2 waybar
```

### Hyprland Integration

```bash
# Generate themes
./huegen ~/Pictures/current_wallpaper.jpg

# Include in hyprland.conf
echo "source = ~/.config/huegen/themes/hyprland" >> ~/.config/hypr/hyprland.conf

# Reload hyprland
hyprctl reload
```

## Contributing

### Code Style

- Use 2-space indentation
- Follow existing naming conventions
- Add comments for complex algorithms

### Adding New Features

1. Color format support: Extend `colorsToJson()` function
2. New color selection algorithms: Modify `selectMostDistinctColors()`
3. Additional integrations: Add hook functions in `hooks.cpp`

### Version 1.0.0

- Initial release
- K-means color extraction
- Template processing system
- Support for 7 color formats
- 16-color palette generation
