from distutils.core import setup
import py2exe
  
setup(
	console=[{"script":"nrfflasher.py"}], 
	options={"py2exe": {
		"includes":["serial", "os", "sys", "time"],
		"dll_excludes":["w9xpopen.exe"],
		"compressed": 1, 
		"optimize": 2, 
		"bundle_files": 1}},
	zipfile=None
)
