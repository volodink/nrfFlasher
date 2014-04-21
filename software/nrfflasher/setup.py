from distutils.core import setup
import py2exe
  
setup(
    console=[{"script":"nrfflasher.py"}], 
    options={"py2exe": {"includes":["serial", "os", "sys", "time"]}},
    zipfile=None
)
