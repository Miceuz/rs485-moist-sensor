try:
    from setuptools import setup
except ImportError:
	from distutils.core import setup

setup(
	name = 'chirp_modbus',
	version = '1.0.1',
	license = 'Apache License, Version 2.0',
	author = 'Albertas Mickenas',
	author_email = 'mic@wemakethings.net',
	url = 'https://github.com/Miceuz/rs485-moist-sensor',
	description = 'A wrapper library for communicatin with Modbus based Chirp soil moisture sensor',
	install_requires = ['minimalmodbus'],
	py_modules = ['chirp_modbus'],
	keywords =['chirp', 'catnip', 'soil', 'moisture', 'sensor', 'rs485'],
	classifiers = [
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Information Technology',
        'Intended Audience :: Science/Research',
        'Intended Audience :: Manufacturing',
        'License :: OSI Approved :: Apache Software License',
        'Natural Language :: English',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Topic :: Communications',
        'Topic :: Home Automation',
        'Topic :: Scientific/Engineering',
        'Topic :: Software Development :: Libraries',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: System :: Hardware :: Hardware Drivers',
        'Topic :: Terminals :: Serial',
	]
)