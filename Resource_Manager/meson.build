app9_resources = gnome.compile_resources('app_resources',
  'mainapp.gresource.xml',
  source_dir: 'schema/')

app9_schemas = gnome.compile_schemas()

executable('exampleapp9',
  'src/main.c',
  'src/mainApp.c',
  'src/mainAppWindow.c',
  'src/mainAppPreferences.c',
  app9_resources,
  app9_schemas,
  dependencies: libgtk_dep,
  c_args: common_cflags)
