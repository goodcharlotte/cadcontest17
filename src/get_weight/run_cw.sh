case_num=$1
out_v_file=$2

DEFAULT_CASE_NUM="1"
DEFAULT_OUT_V="out.v"

if [ -z $case_num ]; then
    case_num=$DEFAULT_CASE_NUM
fi

if [ -z $out_v_file ]; then
	out_v_file=$DEFAULT_OUT_V
fi


./get_wei ../../testcase/unit$case_num/weight.txt  ../$out_v_file

