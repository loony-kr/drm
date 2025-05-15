project('tmpdf-drm', 'cpp')

openssl_dep = declare_dependency(
    include_directories: include_directories('C:\Program Files\OpenSSL-Win64/include'),
    link_args: ['-L' + 'C:\Program Files\OpenSSL-Win64\lib']
)

srcs = [
    'main.cpp',
    'core/encrypt/tmpdf_encryptor.cpp'
]

executable('main', srcs, dependencies: openssl_dep)