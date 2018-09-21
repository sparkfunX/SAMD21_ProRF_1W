# A file parser for the SAMD21 ProRF 1W Range Test Data
# Author: Owen Lyke Sep 2018
# This software is free to use and modify. 

# povide the target file name as the first command line argument

import sys # for command-line arguments
import re  # for easier text parsing

rx_dict = {
    'line': re.compile(r'^(?P<line>[\d]+),', re.MULTILINE),
    'systime' : re.compile(r'SysTime \(ms\): (?P<systime>[\d]+),', re.MULTILINE),
    'gps_stat' : re.compile(r'GPS_STAT: (?P<gps_stat>[\d]+),', re.MULTILINE ), 
    'lat' : re.compile(r'LAT: (?P<lat>[-\d]+\.[\d]+),', re.MULTILINE ),
    'lng' : re.compile(r'LNG: (?P<lng>[-\d]+\.[\d]+),', re.MULTILINE ),
    'hour' : re.compile(r'Hour: (?P<hour>[\d]+),', re.MULTILINE ),
    'minute' : re.compile(r'Minute: (?P<minute>[\d]+),', re.MULTILINE ),
    'second' : re.compile(r'Second: (?P<second>[\d]+),', re.MULTILINE ),
    'packet' : re.compile(r'Packet: \'(?P<packet>(.|\s)*?)\',', re.MULTILINE),
    'rssi' : re.compile(r'RSSI: (?P<rssi>[-\d]+),', re.MULTILINE ),
    'pcktok' : re.compile(r'PcktOK: (?P<pcktok>\d),', re.MULTILINE ),
    'ttlok' : re.compile(r'TotalOK: (?P<ttlok>[\d]+),', re.MULTILINE ),
    # 'ttlbad' : re.compile(r'TotalBad: (?P<ttlbad>[\d]+)$', re.MULTILINE ),
    'ttlbad' : re.compile(r'TotalBad: (?P<ttlbad>[\d]+)', re.MULTILINE ),
    'supah' : re.compile(r'^(?P<line>[\d]+), SysTime \(ms\): (?P<systime>[\d]+), GPS_STAT: (?P<gps_stat>[\d]+), LAT: (?P<lat>[-\d]+\.[\d]+), LNG: (?P<lng>[-\d]+\.[\d]+), Hour: (?P<hour>[\d]+), Minute: (?P<minute>[\d]+), Second: (?P<second>[\d]+), Packet: \'(?P<packet>(.|\s)*?)\', RSSI: (?P<rssi>[-\d]+), PcktOK: (?P<pcktok>\d), TotalOK: (?P<ttlok>[\d]+), TotalBad: (?P<ttlbad>[\d]+)$', re.MULTILINE ),
}

line_list = []
systime_list = []
gps_stat_list = []
lat_list = []
lng_list = []
hour_list = []
minute_list = []
second_list = []
packet_list = []
rssi_list = []
pcktok_list = []
ttlok_list = []
ttlbad_list = []






#
#	Program Start!
#
print ' '

infile_name = sys.argv[1]
with open(infile_name, 'rb') as file:				# This line opens the file, calls it file, then runs the indented code and no matter what it makes sure to close the file afterward!

    file_contents = file.read()	# Returns the file as a (byte?) string

line_list = rx_dict['line'].findall(file_contents)					# Parse the file into the proper lists
systime_list = rx_dict['systime'].findall(file_contents)
gps_stat_list = rx_dict['gps_stat'].findall(file_contents)
lat_list = rx_dict['lat'].findall(file_contents)
lng_list = rx_dict['lng'].findall(file_contents)
hour_list = rx_dict['hour'].findall(file_contents)
minute_list = rx_dict['minute'].findall(file_contents)
second_list = rx_dict['second'].findall(file_contents)
packet_list_temp = rx_dict['packet'].findall(file_contents)
rssi_list = rx_dict['rssi'].findall(file_contents)
pcktok_list = rx_dict['pcktok'].findall(file_contents)
ttlok_list = rx_dict['ttlok'].findall(file_contents)
ttlbad_list = rx_dict['ttlbad'].findall(file_contents)


# Further conditioning on the packet list
for e in packet_list_temp:
	packet_list.append(e[0])


# Check that all the lists have the same number of elements

# list_list = [line_list, systime_list, gps_stat_list, lat_list, lng_list, hour_list, minute_list, second_list, packet_list, rssi_list, pcktok_list, ttlok_list, ttlbad_list];
list_list =   [line_list, systime_list, gps_stat_list, lat_list, lng_list, hour_list, minute_list, second_list,              rssi_list, pcktok_list, ttlok_list, ttlbad_list];
one_length = len(line_list)
for l in list_list:
	if len(l) != one_length:
		print 'Warning: list length mismatch'

# Use regex again to make an output file name
r_infile = re.compile(r'[\w]+')
prefix = r_infile.search(infile_name)
outfile_name = prefix.group(0) + '.csv'

# Open the output file, based on the input file name
with open(outfile_name, 'w') as file:

	file.write('Line #, SysTime (ms), GPS Stat, Lat, Lng, Hour, Minute, Second, RSSI, Packet OK, Total OK, Total Bad\n')

	indi = 0
	while indi < len(line_list):
		for l in list_list:
			# print l[indi]	
			file.write(l[indi] + ',')
		file.write('\n')
		indi += 1

# Tell the user of the success
print str(indi) + ' lines read successfully'
print 'Output created in file: ' + outfile_name