#!/bin/bash -eu

prefix='Automata_HW1_2022-28022'

rm -rf test/generated
python3 -m pip install -r test-requirements.txt
scripts/generate_test.py "test/test.in"

mv test/generated .
rm -rf test
mv generated test

cp scripts/compile.sh "${prefix}_compile.sh"
for i in 1 2; do
	file="${prefix}_Q${i}.sh"
	echo "#!/bin/sh
bin/Q${i}" >>"$file"
	chmod +x "$file"
done

zip -r "${prefix}.zip" ./*.{sh,pdf} CMakeLists.txt src test
