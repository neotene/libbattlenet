project('bnetcpp', 'cpp', version : '0.1',
  default_options : ['cpp_std=c++latest'])

override = []

if host_machine.system() == 'windows'
  override = ['cpp_std=vc++20']
else
  override = ['cpp_std=c++26']
endif

tests = dependency('gtest')
tests_main = dependency('gtest_main')

network = dependency('boost')

openssl = dependency('openssl')

bnetcpp_deps = [network, openssl]

if host_machine.system() == 'linux'
  bnetcpp_deps += dependency('threads')
endif

bnetcpp_src = [
  'src/client.cpp'
]

type = get_option('default_library')

bnetcpp_cpp_args = []
cpp_args = []

if type == 'static'
    cpp_args += ['-DBNETCPP_STATIC']
else
    bnetcpp_cpp_args += ['-DBNETCPP_BUILDING_THE_LIB']
endif

tests_cpp_args = []

bnetcpp_incs = include_directories(['include/bnetcpp'])

bnetcpp = library('bnetcpp',
  bnetcpp_src,
  dependencies : bnetcpp_deps,
  cpp_args: bnetcpp_cpp_args + cpp_args,
  include_directories: bnetcpp_incs,
  override_options: override
)

bnetcpp_dep = declare_dependency(
  include_directories : bnetcpp_incs,
  link_with : bnetcpp,
)

test('test_all',
  executable('test_all',
    ['test_all.cpp'],
    dependencies: [bnetcpp_dep, tests, network],
    cpp_args: tests_cpp_args + cpp_args,
    include_directories: include_directories(['include']),
    override_options: override
  ),
  args : ['--gtest_color=yes'])
