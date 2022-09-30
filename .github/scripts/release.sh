#!/bin/bash -eu

hwnum="$1"

scripts/cleanup.sh test
scripts/preprocess.py "test/test.in"

cp scripts/compile.sh "Automata_HW${hwnum}_2022-28022_compile.sh"
for i in 1 2; do
	file="Automata_HW${hwnum}_2022-28022_Q${i}.sh"
	echo "#!/bin/sh
bin/Q${i}" >>"$file"
	chmod +x "$file"
done

zip -r "Automata_HW${hwnum}_2022-28022.zip" \
	./*.sh CMakeLists.txt src test/*.{q,a}*
