# -*- coding:utf-8 -*-

import sys
from distutils.core import setup

python_version = sys.version[:3].replace(".", "")

setup(
    name='othello',
    version='0.1.0',
    description='Python Othello Module C++ Implement',
    author='khml',
    author_email='',
    url='https://github.com/khml/Othello',
    install_requires=['numpy'],
    packages=[''],
    py_modules=[''],
    package_data={'': ['othello.cpython-{}m-darwin.so'.format(python_version)]},
)
