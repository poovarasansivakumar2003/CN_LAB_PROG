# Creates a new NS2 simulation instance.
set ns [new Simulator]

# Save simulation logs to 3.tr (text trace) and 3.nam (animation view in NAM tool).
set trace_file [open 3.tr w]
$ns trace-all $trace_file
set nam_file [open 3.nam w]
$ns namtrace-all $nam_file

# The below code is used to create the nodes.
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

# This is used to give color and label to the packets.
# n0, n1: source nodes (sending ping and UDP traffic)
# n4, n5: destinations for ping replies
# n2, n3: intermediate routers
$ns color 1 "blue"
$ns color 2 "orange"
$n0 label "Ping0"
$n1 label "Ping1"
$n4 label "Ping4"
$n5 label "Ping5"

# Create Full Duplex Links Between Nodes
# Format: ns duplex-link source dest bandwidth delay queuingPolicy
$ns duplex-link $n0 $n2 0.5mb 10ms DropTail
$ns duplex-link $n1 $n2 0.5mb 10ms DropTail
$ns duplex-link $n2 $n3 0.5mb 10ms DropTail
$ns duplex-link $n3 $n4 0.5mb 10ms DropTail
$ns duplex-link $n3 $n5 0.5mb 10ms DropTail

# connect the ping agents
set ping0 [new Agent/Ping]
$ns attach-agent $n0 $ping0
set ping1 [new Agent/Ping]
$ns attach-agent $n1 $ping1
set ping4 [new Agent/Ping]
$ns attach-agent $n4 $ping4
set ping5 [new Agent/Ping]
$ns attach-agent $n5 $ping5

#The below code is used to set the packet size and interval.
$ping0 set packetSize_ 500
$ping0 set interval_ 0.001
$ping1 set packetSize_ 500
$ping1 set interval_ 0.001
$ping4 set packetSize_ 500
$ping4 set interval_ 0.001
$ping5 set packetSize_ 500
$ping5 set interval_ 0.001

# UDP + CBR Setup
set udp0 [new Agent/UDP]
set cbr [new Application/Traffic/CBR]
$ns attach-agent $n0 $udp0
$cbr attach-agent $udp0
$cbr set packetSize_ 512  ;#CBR traffic app sends 512-byte packets every 1 ms
$cbr set interval_ 0.001

set null [new Agent/Null]
$ns attach-agent $n4 $null
$ns connect $udp0 $null

# This code is used give a color ping0 is blue and ping1 is orange
$ping0 set class_ 1
$ping1 set class_ 2

# Connect Agents for Ping and UDP
$ns connect $ping0 $ping4
$ns connect $ping1 $ping5

#The below function is executed when the ping agent receives a reply from the destination
Agent/Ping instproc recv {from rtt} { 
$self instvar node_ 
puts " The node [$node_ id] received a reply from $from with round trip time of $rtt ms"
}

#define finish procedure
proc finish { } {
global ns nam_file trace_file
$ns flush-trace
exec nam 3.nam &
close $trace_file
close $nam_file
exit 0
}

#schedule events to start sending the ping packets
$ns at 0.1 "$ping0 send"
$ns at 0.2 "$ping0 send"
$ns at 0.3 "$ping0 send"
$ns at 0.4 "$ping0 send"
$ns at 0.5 "$ping0 send"
$ns at 0.6 "$ping0 send"
$ns at 0.7 "$ping0 send"
$ns at 0.8 "$ping0 send"
$ns at 0.9 "$ping0 send"
$ns at 1.0 "$ping0 send"
$ns at 0.2 "$cbr start"
$ns at 4.0 "$cbr stop"
$ns at 0.1 "$ping1 send"
$ns at 0.2 "$ping1 send"
$ns at 0.3 "$ping1 send"
$ns at 0.4 "$ping1 send"
$ns at 0.5 "$ping1 send"
$ns at 0.6 "$ping1 send"
$ns at 0.7 "$ping1 send"
$ns at 0.8 "$ping1 send"
$ns at 0.9 "$ping1 send"
$ns at 1.0 "$ping1 send"
$ns at 5.5 "finish"
$ns run