#!/usr/bin/env python3
"""Black-box tests against a compiled StreamMatch executable; no extra packages."""
from pathlib import Path
import subprocess
import sys


def main() -> int:
    if len(sys.argv) != 2:
        print("Usage: python tests/test_cli.py <path-to-streammatch>")
        return 2
    executable = Path(sys.argv[1]).resolve()
    if not executable.is_file():
        print(f"Executable not found: {executable}")
        return 2
    cases = [
        ("C01 clean exit", "0\n", ["Goodbye."], []),
        ("C02 browse", "1\n0\n", ["FICTIONAL CATALOGUE (12 titles)", "12. Small Town Stage"], []),
        ("C03 explain", "3\n0\n", ["not a probability", "rule-based classroom"], []),
        ("C04 standard match", "2\n23\n2\n1\n95\n0\n", ["1. Campus Detour | score 8/8", "2. Weekend Mix-Up | score 8/8"], []),
        ("C05 no eligible titles", "2\n23\n1\n1\n30\n0\n", ["No eligible titles"], ["TOP MATCHES"]),
        ("C06 one eligible title", "2\n0\n4\n1\n80\n0\n", ["1. Robot Roommate | score 8/8"], ["2. Weekend", "TOP MATCHES (up to 2)"]),
        ("C07 honest alternatives", "2\n23\n4\n3\n100\n0\n", ["No exact genre-and-mood match", "1. Robot Roommate | score 5/8", "mood differs"], []),
        ("C08 invalid menu then recover", "word\n9\n0\n", ["Invalid input.", "Goodbye."], []),
        ("C09 invalid age then recover", "2\n-1\n121\n23\n2\n1\n95\n0\n", ["whole number from 0 to 120", "Campus Detour"], []),
        ("C10 invalid genre then recover", "2\n23\n0\n5\n2\n1\n95\n0\n", ["whole number from 1 to 4", "Campus Detour"], []),
        ("C11 invalid mood then recover", "2\n23\n2\n0\n4\n1\n95\n0\n", ["whole number from 1 to 3", "Campus Detour"], []),
        ("C12 invalid time then recover", "2\n23\n2\n1\n29\n241\n95\n0\n", ["whole number from 30 to 240", "Campus Detour"], []),
        ("C13 decimal and trailing text", "2.0\n2abc\n0\n", ["Invalid input.", "Goodbye."], []),
        ("C14 blank and overflow", "\n999999999999999999999\n0\n", ["Invalid input.", "Goodbye."], []),
        ("C15 EOF at menu", "", ["Input ended."], []),
        ("C16 EOF during questions", "2\n23\n2\n", ["Input ended."], []),
        ("C17 surrounding whitespace", " 2 \n 23 \n 2 \n 1 \n 95 \n 0 \n", ["Campus Detour", "Goodbye."], ["Invalid input."]),
        ("C18 repeat without restarting", "2\n23\n2\n1\n95\n2\n0\n4\n1\n80\n0\n", ["1. Campus Detour", "1. Robot Roommate", "Goodbye."], []),
    ]
    failed = 0
    for name, input_text, required, prohibited in cases:
        try:
            result = subprocess.run([str(executable)], input=input_text, text=True,
                                    capture_output=True, timeout=5, check=False)
            success = (result.returncode == 0 and not result.stderr
                       and all(text in result.stdout for text in required)
                       and all(text not in result.stdout for text in prohibited))
        except (OSError, subprocess.TimeoutExpired) as error:
            print(f"FAIL {name}: {error}")
            failed += 1
            continue
        print(f"{'PASS' if success else 'FAIL'} {name}")
        if not success:
            failed += 1
            print(result.stdout, result.stderr)
    print(f"\nRESULT: {len(cases) - failed} passed, {failed} failed.")
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
