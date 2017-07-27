case_num=$1
if [ -z $1 ]; then
    case_num="1"
fi
./rpgen ../testcase/unit$case_num/F.v ../testcase/unit$case_num/G.v ../testcase/unit$case_num/weight.txt patch.v out.v
cat patch.v > final.v
cat out.v >> final.v
cp ../testcase/unit$case_num/G.v Golden.v
./abc -f verify.script
