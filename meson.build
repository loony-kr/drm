project('tmpdf-drm', 'cpp')

openssl_dep = dependency('openssl', required: true)

srcs = [
    'main.cpp',
    'core/encrypt/tmpdf_encryptor.cpp'
]

executable('drpdf_encryptor', srcs, dependencies: openssl_dep)