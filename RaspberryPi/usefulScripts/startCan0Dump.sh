#These commands can be run in a larger script in the background
#but doing so runs the risk of the script being run twice,
#with the result of having multiple copies of the process running
#concurrently, which could have undesired behavior.
candump -tA can0 >> can0Log.txt
