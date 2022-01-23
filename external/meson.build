if host_machine.system() == 'linux'
  subdir('include/linux')
endif

if host_machine.system() == 'windows'
  external_inc_dirs = include_directories('include',
                                          'include/GL',
                                          'include/allegro',
                                          'include/allegrogl',
                                          'include/boost_1_75/',
                                          'include/fmod',
                                          'include/luabind')
endif
subdir('lib')
subdir('sources')
