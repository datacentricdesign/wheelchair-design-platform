from subprocess import Popen, PIPE		# Necessary modules


command = "ping -c 5 google.com"

"""-------------------------------------------------------
	run function opens up a shell process and runs 
	command in it, capturing its stdout, one line at 
	a time. 
-------------------------------------------------------"""
def run(command):						   
    process = Popen(command, stdout=PIPE, shell=True)
    while True:	
        line = process.stdout.readline().rstrip()
        if not line:		
	            break	
        yield line


"""-------------------------------------------------------
	parse function checks to see if sub_string argument
	is in input string, and if it is, do the parsing
-------------------------------------------------------"""
def parse(input_str, sub_str):						
	if sub_str in input_str:
		get_rssi(input_str)	


"""-------------------------------------------------------
	remove rssi, and print it
-------------------------------------------------------"""
def get_rssi(input_str):
	# splitting string at rssi						
	lhs, rhs = input_str.split("google.", 1)
	print(lhs)
	print(rhs)


"""-------------------------------------------------------
	If file is the main one being run, call run function
	and iterate over each stdout line.
-------------------------------------------------------"""
if __name__ == "__main__":    
    for line in run(command):
        parse(str(line), str("google"))


