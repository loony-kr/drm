project('drm', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

openssl_dep = dependency('openssl', required: true)
nlohmann_json = dependency('nlohmann_json')
sqlite_dep = dependency('sqlite3')

shared_srcs = [
  'core/encrypt/encryptor.cpp',
  'core/decrypt/decryptor.cpp',
  'core/filesystem/filesystem.cpp',
  'core/base/baseDRM.cpp',
  'core/database/database.cpp',
  'core/hmac/hmac.cpp',
  'core/random/random.cpp',
  'core/env/env.cpp',
  'core/base64/base64.cpp',
  'core/rsa/rsa.cpp',
]

encryptor_srcs = shared_srcs + ['main.cpp']
server_srcs = shared_srcs + ['server.cpp']

executable('encryptor',
  encryptor_srcs,
  dependencies : [openssl_dep, nlohmann_json, sqlite_dep],
  include_directories : include_directories('core'),
  link_args : ['-L/opt/homebrew/opt/openssl@3/lib', '-lssl', '-lcrypto']
)

executable('server',
  server_srcs,
  dependencies : [openssl_dep, nlohmann_json, sqlite_dep],
  include_directories : include_directories('core'),
  link_args : ['-L/opt/homebrew/opt/openssl@3/lib', '-lssl', '-lcrypto']
)