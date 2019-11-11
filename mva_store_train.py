#!/bin/env python

#import sys
import os
import subprocess


def do_store( exe_name ):

	print( "The store cmd is {} ".format(exe_name) )

	cmd1 = "{} /wk_cms2/cychuang/full_sel_file_2016legacy/TTbar/full_sel_bpk_ntuple_[0-9] /wk_cms2/cychuang/trained_files/{}_10 | tee /wk_cms2/cychuang/trained_files/{}_10.log".format( exe_name , exe_name , exe_name  )
	os.popen( cmd1 )

	cmd2 = "{} /wk_cms2/cychuang/full_sel_file_2016legacy/TTbar/full_sel_bpk_ntuple_*[0-4] /wk_cms2/cychuang/trained_files/{}_0_4 | tee /wk_cms2/cychuang/trained_files/{}_0_4.log".format( exe_name , exe_name , exe_name )
	os.popen( cmd2 )

	cmd3 = "{} /wk_cms2/cychuang/full_sel_file_2016legacy/TTbar/full_sel_bpk_ntuple_*[5-9] /wk_cms2/cychuang/trained_files/{}_5_9 | tee /wk_cms2/cychuang/trained_files/{}_5_9.log".format( exe_name , exe_name , exe_name )
	os.popen( cmd3 )

def do_train( num_of_train ):

	print( "The training file is train_{}.cc".format( num_of_train ) )
	
	cmd4 = "root /wk_cms2/cychuang/CMSSW_9_4_2/src/wk_mva/train_{}.cc | tee /wk_cms2/cychuang/train_result/t{}.log".format( num_of_train, num_of_train )
	os.popen( cmd4 )



def do_all( argv ):
	
	if len(argv) == 2:
		do_store( argv[1] )
	elif len(argv) == 3:
		do_store( argv[1] )
		do_train( argv[2] )
	else:
		print("Illegal usage! ( $./exe <storecmd> [# of train_#.cc] )")	
			


if __name__ == '__main__':
	from sys import argv
	do_all(argv)

