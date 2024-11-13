project('bnetcpp', 'cpp', version : '0.1',
  default_options : ['cpp_std=c++20'])

tests = dependency('gtest')
tests_main = dependency('gtest_main')

network = dependency('boost')

openssl = dependency('openssl')

bnetcpp_deps = [network]

if host_machine.system() == 'linux'
  bnetcpp_deps += dependency('threads')
endif

bnetcpp_src = [
  'src/auth.cpp',
]

type = get_option('default_library')

bnetcpp_cpp_args = []
cpp_args = []

if host_machine.system() == 'windows'
    cpp_args += ['-D_WIN32_WINNT=_WIN32_WINNT_WIN10', '-D_WINSOCK_DEPRECATED_NO_WARNINGS']
endif

if type == 'static'
    cpp_args += ['-DBNETCPP_STATIC']
else
    bnetcpp_cpp_args += ['-DBNETCPP_BUILDING_THE_LIB']
endif

tests_cpp_args = []

bnetcpp_incs = include_directories(['include'])

bnetcpp = library('bnetcpp',
  bnetcpp_src,
  dependencies : bnetcpp_deps,
  cpp_args: bnetcpp_cpp_args + cpp_args,
  include_directories: bnetcpp_incs
)

bnetcpp_dep = declare_dependency(
  include_directories : bnetcpp_incs,
  link_with : bnetcpp,
)

test('test_all',
  executable('test_all',
    ['test_all.cpp'],
    dependencies: [bnetcpp_dep, tests, tests_main],
    cpp_args: tests_cpp_args + cpp_args,
  ),
  args : ['--gtest_color=yes'])
