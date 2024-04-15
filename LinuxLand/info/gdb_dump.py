import os
import functools

import gdb

def dump_display_csv(f, buffer, width, height):
	arr = gdb.parse_and_eval(buffer)
	for y in range(height):
		for x in range(width):
			offset = y*height + x
			print(f"{int(arr[offset])}, ", end="", file=f)
			print(file=f)


@functools.cache
def pallete_lookup(palette, idx):
	p = pallete[idx]
	return b''.join(int(p[colour]).to_bytes(1,'little') for colour in "rgb")

def dump_display_with_palette(f, buf, palette, width, height):
	arr = gdb.parse_and_eval(buf)
	pal = gdb.parse_and_eval(palette)

	for y in range(height):
		for x in range(width):
			offset = y*height+x
			pixel = int(arr[offset])
			f.write(value)


class DoomDumpDisplay(gdb.Command):
	def __init__(self):
		super().__init__("doom-dump-frame", gdb.COMMAND_NONE)

	def invoke(self, arg, from_tty):
		with open(arg, "w") as f:
			dump_display_csv(f, "I_VideoBuffer",320,200)

DoomDumpDisplay()


class DoomShowDisplay(gdb.Command):
	def __init__(self):
		super().__init__("doom-show-frame", gdb.COMMAND_NONE)

	def invoke(self, arg, from_tty):
		with open(arg, "w") as f:
			dump_display_csv(f, "I_VideoBuffer",320,200)
		
		os.system("convert -depth 8 -size 320x200 rgb:doom.rgb doom.png")
		os.system("kolourpaint doom.png")

DoomShowDisplay()


