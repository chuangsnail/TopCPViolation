#!/bin/env python

#import sys
import os
import subprocess


def do_store( st_name ):

	print( "The store cmd is {} ".format(st_name) )

	cmd1 = "st_tp_part /wk_cms2/cychuang/16_full_SR/TTbar/full_sel_bpk_ntuple*[0-2] /wk_cms2/cychuang/trained_files/{}_0-2 | tee /wk_cms2/cychuang/trained_files/{}_0-2.log".format( st_name , st_name  )
	os.popen( cmd1 )

	cmd2 = "st_tp_part /wk_cms2/cychuang/16_full_SR/TTbar/full_sel_bpk_ntuple*[3-6] /wk_cms2/cychuang/trained_files/{}_3-6 | tee /wk_cms2/cychuang/trained_files/{}_3-6.log".format( st_name , st_name )
	os.popen( cmd2 )

	cmd3 = "st_tp_part /wk_cms2/cychuang/16_full_SR/TTbar/full_sel_bpk_ntuple*[7-9] /wk_cms2/cychuang/trained_files/{}_7-9 | tee /wk_cms2/cychuang/trained_files/{}_7-9.log".format( st_name , st_name )
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

