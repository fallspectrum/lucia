class Grid_Click_Info:
	def __init__(self,position,x,y):
		self.index = position;
		self.rel_x = x;
		self.rel_y = y;
	
	def get_index(self):
		return self.index;
	
	def get_rel_x(self):
		return self.rel_x

	def get_rel_y(self):
		return self.rel_y;

class Utils:

	#Base on spacing of a item returns the index
	#of the element clicked. Also returns the
	#relative x and y position of the index clicked.
	#the spacing of elements is added to each element's
	#dimensions
	@staticmethod
	def get_grid_click_info(
		row_count,
		element_width,
		element_height,
		element_width_spacing,
		element_height_spacing,
		(click_x,click_y)):

		total_element_width = element_width + element_width_spacing
		total_element_height = element_height + element_height_spacing

		y_pos = click_y / (total_element_width)
		x_pos = click_x /  (total_element_height)
		container_index = y_pos * row_count;
		container_index += x_pos;
		
		#calculates the relative x and y offset in the item that was clicked
		rel_x = click_x - (x_pos * total_element_width)
		rel_y = click_y - (x_pos * total_element_height)

		return Grid_Click_Info(container_index,rel_x,rel_y);




	#converts byte value to hex string
	@staticmethod 
	def byte_to_hex(val):
		return Utils.value_to_hex(val,1)

	#converts short value to string
	#always outputs two bytes
	@staticmethod 
	def short_to_hex(val):
		return Utils.value_to_hex(val,2)

	#converts dword value to string
	#always outputs 4 bytes
	@staticmethod
	def dword_to_hex(val):
		return Utils.value_to_hex(val,4)

	@staticmethod
	def value_to_hex(val,minByteCount=4):
		returnVal = ""
		curVal = val;
		digitCount = 0 
		nibbles = [0,0]
		byteCount = 0
		while byteCount < minByteCount:
			while digitCount < 2:
				tempVal  = curVal & 0x0f
				if(tempVal > 9):
					tempVal -= 10;
					nibbles[digitCount] = chr(97+tempVal)
				else:
					nibbles[digitCount] = str(tempVal)
				digitCount += 1
				curVal = curVal >> 4
			digitCount = 0
			returnVal += '0x' + nibbles[1] + nibbles[0] + ","
			byteCount += 1
		return returnVal

	#Inserts the given value in the middle of the list
	#Error occurs when there are duplicate values
	#returns index on success, -1 on error
	#TODO specify duplicate value handling method
	@staticmethod
	def findInsertIndex(aList,aVal):
		listLength = len(aList);
		insertIndex = -1
		if(listLength == 0):
			insertIndex = 0
		elif(listLength == 1):
			if(aVal < aList[0]):
				insertIndex = 0
			elif(aVal > aList[0]):
				insertIndex = 1
			else:
				insertIndex = -1
		else:
			lowerIndex = 0
			upperIndex = upperBound = listLength-1
			oddLen = False
			
			while(lowerIndex <> upperIndex):
				dI = upperIndex - lowerIndex;
				if(dI == 1):
					if(aVal < aList[lowerIndex]):
						insertIndex = lowerIndex
					elif(aVal > aList[upperIndex]):
						insertIndex = upperIndex+1
					elif(aVal == aList[upperIndex] or
						aVal == aList[lowerIndex]):
							insertIndex = -1
					else:
						insertIndex = lowerIndex+1
					break
				
				middleIndex = upperIndex - (dI/2)
				if(aVal < aList[middleIndex]):
					upperIndex = middleIndex -1
				elif(aVal > aList[middleIndex]):
					lowerIndex = middleIndex +1
				else:
					insertIndex = middleIndex = lowerIndex = upperIndex = -1
					break
			if(lowerIndex == upperIndex and lowerIndex <> -1):
				if(aVal == aList[lowerIndex]):
					insertIndex = -1
				if(aVal < aList[lowerIndex]):
					insertIndex = lowerIndex
				else:
					insertIndex = lowerIndex + 1
		return insertIndex


