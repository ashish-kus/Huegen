#!/bin/bash

# DEFAULT_WALLPAPER_DIR="$HOME/Pictures/Wallpapers"
DEFAULT_WALLPAPER_DIR="$HOME/Pictures/MinecraftWallpapers/"

if ! pgrep -x "swww-daemon" >/dev/null; then
  swww-daemon
fi

if [[ -d "$1" ]]; then
  WALLPAPER_DIR="$1"
  SELECTED_WALLPAPER=$(find "$WALLPAPER_DIR" -type f | shuf -n 1)
elif [[ -f "$1" ]]; then
  SELECTED_WALLPAPER="$1"
else
  WALLPAPER_DIR="$DEFAULT_WALLPAPER_DIR"
  SELECTED_WALLPAPER=$(find "$WALLPAPER_DIR" -type f | shuf -n 1)
fi

if [[ ! -f "$SELECTED_WALLPAPER" ]]; then
  notify-send "❌ Error: No valid wallpaper found!"
  exit 1
fi

ln -sf "$SELECTED_WALLPAPER" "$HOME/.wallpaper.png"

CURSOR_POS="$(hyprctl cursorpos)"

TRANSITION_BEZIER="0.0,0.0,1.0,1.0"
TRANSITION_FPS=60
TRANSITION_TYPE="any"
TRANSITION_DURATION=0.7

swww img "$SELECTED_WALLPAPER" \
  --transition-bezier "$TRANSITION_BEZIER" \
  --transition-fps="$TRANSITION_FPS" \
  --transition-type="$TRANSITION_TYPE" \
  --transition-duration="$TRANSITION_DURATION" \
  --transition-pos "$CURSOR_POS"

~/.binary/heugen ~/.wallpaper.png
cat ~/.config/mako/base.conf ~/.config/huegen/themes/colors-mako.conf >~/.config/mako/config
makoctl reload

sleep 0.5
pkill -SIGUSR2 waybar

ps -t $(who | awk '{print $2}') -o pid=,comm= | grep -E 'zsh|bash' | awk '{print $1}' | while read pid; do
  kill -USR1 "$pid"
done

for i in {0..7}; do
  printf "\e[48;5;${i}m   \e[0m" # 8 spaces wide
done
echo

for i in {8..15}; do
  printf "\e[48;5;${i}m   \e[0m" # 8 spaces wide
done
echo

~/.config/huegen/themes/termcol.sh
