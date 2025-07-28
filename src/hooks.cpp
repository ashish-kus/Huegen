#include "hooks.hpp"
#include <cstdlib>
#include <iostream>

void reloadWaybar() {
  int status = std::system("pkill -SIGUSR2 waybar");
  std::cout << (status == 0 ? "Reloaded Waybar" : "Failed to reload Waybar")
            << std::endl;
}

void reloadHyprland() {
  int status = std::system("hyprctl reload");
  std::cout << (status == 0 ? "Reloaded Hyprland" : "Failed to reload Hyprland")
            << std::endl;
}

void notifyUser(const std::string &message) {
  std::string cmd = "notify-send 'PostInstall' '" + message + "'";
  std::system(cmd.c_str());
}
