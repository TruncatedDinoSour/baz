# the index file

the index file ( `baz.env` ) defines all metadata
about your plugin, it should have these variables :

- `BAZP_NAME` -- the plugin name
- `BAZP_LICENSE` -- the plugin license
- `BAZP_VER` -- the plugin version ( a number incrementing by 1 every version [1, 2, 3, ... 65, 66, 67, ...] )
- `BAZP_AUTHOR` -- the author of the plugin ( can be your full name, your username, email, whatever )
- `BAZP_SRC ` -- the source / plugin folder ( see [plugin folder structure docs](/doc/PLUGIN_FOLDER_STRUCTURE.md) )
- `BAZP_METHOD` -- the recommended method to install this plugin ( like git, local, ... )
- `BAZP_SOURCE` -- the source of your plugin ( might be an external link, a directory name ( like `.` for example ), a GitHub repo, anything )
- `BAZP_DESCRIPTION` -- the description of your plugin
