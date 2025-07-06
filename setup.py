from setuptools import setup, Extension
import pybind11

# 定义扩展模块
ext_modules = [
    Extension(
        'example',  # 模块名称
        sources=['src/main.cpp'],  # 源文件
        include_dirs=[
            pybind11.get_include(),
            pybind11.get_include(True),
            'include'
        ],
        language='c++',
        extra_compile_args=['-std=c++11'],
    ),
]

setup(
    name='example',
    version='0.1',
    description='Pybind11 example',
    ext_modules=ext_modules,
)