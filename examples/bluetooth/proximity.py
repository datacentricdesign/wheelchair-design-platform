from subprocess import Popen, PIPE		# Necessary modules

ready_for_rssi = False
command = "btmon"	# command for scanning
uuid = ""		# uuid of beacon to scan
threshold = 0	# threshold for proximity

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
	is in input string, and if it is, returns false
-------------------------------------------------------"""
def parse(input_str, sub_str):						
	if sub_str in input_str:
		return(True)
	else:
		return(False)


"""-------------------------------------------------------
	remove rssi, and print it
-------------------------------------------------------"""
def get_rssi(input_str):
	# splitting string at rssi	
	global ready_for_rssi					
	lhs, rhs = input_str.split("RSSI: ", 1) 
	print("Command: " + lhs + rhs)
	rssi_value = rhs.split()[0]  # Get first element of 
	# split string composed by elements between spaces
	# of rhs string

	print("RSSI: " + rssi_value)
	ready_for_rssi = False
	return(rssi_value)


"""-------------------------------------------------------
	If file is the main one being run, call run function
	and iterate over each stdout line. if line is of uuid
	start looing for line with RSSI. Once its found, it 
	is retrieved, and it begins to look for uuid again
-------------------------------------------------------"""
if __name__ == "__main__":    
    for line in run(command):
        if parse(str(line), str(uuid)) and not(ready_for_rssi):
        	ready_for_rssi = True

        if ready_for_rssi and ("RSSI: " in str(line)):
        	rssi_value = get_rssi(str(line))
        	if(int(rssi_value) >= threshold):
        		print("You are within your threshold of the beacon!")
        	continue

        elif ready_for_rssi and not ("RSSI: " in line):
        	continue


