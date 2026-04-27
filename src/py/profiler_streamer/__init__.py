import os

from .bindings import profilerStreamerBindings as psb

__all__ = ["psb"]

DLL_PATHS = os.path.join(os.path.dirname(__file__), "bindings")
os.add_dll_directory(DLL_PATHS)