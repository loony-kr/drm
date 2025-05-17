project('drm', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

openssl_dep = dependency('openssl', required: true)
nlohmann_json = dependency('nlohmann_json')
#httplib = dependency('cpp-httplib')
sqlite_dep = dependency('sqlite3')

srcs = [
    'main.cpp',
    'core/encrypt/encryptor.cpp',
    'core/decrypt/decryptor.cpp',
    'core/filesystem/filesystem.cpp',
    'core/base/baseDRM.cpp',
    'core/database/database.cpp',
]

executable('encryptor',
  srcs,
  dependencies : [openssl_dep, nlohmann_json, sqlite_dep],
  include_directories : include_directories('core'),
  link_args : ['-L/opt/homebrew/opt/openssl@3/lib', '-lssl', '-lcrypto']
)

server_srcs = [
    'server.cpp',
    'core/encrypt/encryptor.cpp',
    'core/decrypt/decryptor.cpp',
    'core/filesystem/filesystem.cpp',
    'core/base/baseDRM.cpp',
    'core/database/database.cpp',
]

executable('server',
  server_srcs,
  dependencies : [openssl_dep, nlohmann_json, sqlite_dep],
  include_directories : include_directories('core'),
  link_args : ['-L/opt/homebrew/opt/openssl@3/lib', '-lssl', '-lcrypto']
)