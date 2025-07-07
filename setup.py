from setuptools import setup, Extension
import pybind11

# 定义扩展模块
ext_modules = [
    Extension(
        'graph',  # 模块名称
        sources=['src/main.cpp'],  # 源文件
        include_dirs=[
            pybind11.get_include(),
            pybind11.get_include(True),
            'include'
        ],
        language='c++',
        extra_compile_args=['-std=c++20'],
    ),
]

setup(
    name='graph',
    version='0.1',
    description='Pybind11 example',
    ext_modules=ext_modules,
)