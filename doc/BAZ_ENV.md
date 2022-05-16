# The index file

The index file (`baz.env`) defines all metadata
about your plugin, it should have these variables:

- `BAZP_NAME` -- The plugin name
- `BAZP_LICENSE` -- The plugin license
- `BAZP_VER` -- The plugin version (a number incrementing by 1 every version [1, 2, 3, ... 65, 66, 67])
- `BAZP_AUTHOR` -- The author of the plugin (can be your full name, your username, email, whatever)
- `BAZP_SRC ` -- The source/plugin folder (see [plugin folder structure docs](/doc/PLUGIN_FOLDER_STRUCTURE.md))
- `BAZP_METHOD` -- The recommended method to install this plugin (like git, local, ...)
- `BAZP_SOURCE` -- The source of your plugin (might be an external link, a directory name (like `.` for example), a GitHub repo, anything)
- `BAZP_DESCRIPTION` -- The description of your plugin
