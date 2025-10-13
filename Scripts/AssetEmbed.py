import argparse
import sys
from pathlib import Path
from typing import Iterator


def create_c_array_name(filename: str) -> str:
	stem = Path(filename).stem
	identifier = ''.join(c if c.isalnum() else '_' for c in stem)
	if not identifier or identifier[0].isdigit():
		identifier = f'g_{identifier or "Buffer"}'
	else:
		identifier = f'g_{identifier}'
	return identifier


def format_bytes_as_c_array(data: bytes, bytes_per_line: int = 16) -> Iterator[str]:
	"""Yield formatted hex byte lines for C array output."""
	for i in range(0, len(data), bytes_per_line):
		chunk = data[i:i + bytes_per_line]
		hex_values = ', '.join(f'0x{b:02X}' for b in chunk)
		yield f'    {hex_values},'


def convert_binary_to_c_array(input_path: Path, output_path: Path, array_name: str, include_header: bool = True) -> int:
	"""Convert binary data to a C-style uint8_t array file."""
	data = input_path.read_bytes()
	lines = list(format_bytes_as_c_array(data))

	size_name = f'{array_name}_Size'
	header_lines = []

	if include_header:
		header_lines = [
			f'// Generated from: {input_path.name}',
			f'// Size: {len(data)} bytes',
			'',
			'#include <cstdint>',
			''
		]

	content_lines = [
		*header_lines,
		f'const uint8_t {array_name}[] = {{',
		*lines,
		'};',
		'',
		f'const size_t {size_name} = sizeof({array_name});',
		''
	]

	output_path.write_text('\n'.join(content_lines), encoding='utf-8')
	return len(data)


def main() -> int:
	parser = argparse.ArgumentParser(
		description='Convert binary files to C-style uint8_t arrays for embedding',
		formatter_class=argparse.RawDescriptionHelpFormatter,
		epilog="""
Examples:
  %(prog)s data.bin                    # Creates Buffer.embed
  %(prog)s image.png image_data.embed  # Creates image_data.embed
  %(prog)s -n my_data file.bin         # Uses 'g_my_data' as array name
        """
	)

	parser.add_argument('input_file', help='Input binary file to convert')
	parser.add_argument('output_file', nargs='?', default='Buffer.embed', help='Output .embed file')
	parser.add_argument('-n', '--name', dest='array_name', help='Custom name for the C array variable')
	parser.add_argument('-v', '--verbose', action='store_true', help='Enable verbose output')
	parser.add_argument('--no-header', action='store_true', help='Exclude comment header from output')

	args = parser.parse_args()

	input_path = Path(args.input_file)
	output_path = Path(args.output_file).with_suffix('.embed')
	array_name = args.array_name or create_c_array_name(input_path.name)

	if not input_path.exists() or not input_path.is_file():
		print(f"Error: Input file '{input_path}' does not exist or is not a regular file.", file=sys.stderr)
		return 1

	try:
		if args.verbose:
			print(f"Converting '{input_path}' → '{output_path}'")
			print(f"Array name: {array_name}")

		byte_count = convert_binary_to_c_array(input_path, output_path, array_name, not args.no_header)
		print(f"✅ Converted {byte_count:,} bytes to '{output_path}'")

		if args.verbose:
			print(f"Include in C++: #include \"{output_path.name}\"")

		return 0

	except Exception as e:
		print(f"❌ Error: {e}", file=sys.stderr)
		return 1


if __name__ == '__main__':
	sys.exit(main())
