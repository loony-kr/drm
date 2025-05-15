project('tmpdf-drm', 'cpp')

openssl_dep = dependency('openssl', required: true)
nlohmann_json = dependency('nlohmann_json')
httplib = dependency('cpp-httplib')

srcs = [
    'main.cpp',
    'core/encrypt/tmpdf_encryptor.cpp'
    'core/decrypt/tmpdf_decryptor.cpp'
]

executable('drpdf_encryptor', srcs, dependencies: openssl_dep)