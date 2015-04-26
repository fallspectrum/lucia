from effectgroup import EffectGroup
from effectdescriptions import EffectDescriptions
from myutils import Utils

def generateFile(fileName,ledCount,effectGroups):
	f = open(fileName,'w')
	f.write("#ifndef H_SONG_INSTRUCTIONS\n#define H_SONG_INSTRUCTIONS\n#include \"avr/pgmspace.h\"\n#include \"song_instructions.h\"\nconst char song_instructions[] PROGMEM = {")
	lastTime = 0
	for curEffectGroup in effectGroups:
		writeBuffer = ""
		newTime = curEffectGroup.getTimeAs10msCount()
		tD = newTime - lastTime
		lastTime = newTime
		writeBuffer += "0xff,\n"
		writeBuffer += Utils.short_to_hex(tD) + "\n"
		for ledIndex in range (0,ledCount):
			ledEffect = curEffectGroup.getLedEffect(ledIndex)
			tempLedBytes = generateLedEffectBytes(ledIndex,ledEffect)
			if tempLedBytes <> "":
				writeBuffer += "\t" + generateLedEffectBytes(ledIndex,ledEffect) + "\n"
		writeBuffer +=  "0xff,\n"
		f.write(writeBuffer)
	f.write("0x00,};\n#endif")


#generates a string for led effect
def generateLedEffectBytes(ledNumber,ledEffect):
	effectNumber = ledEffect[EffectGroup.INDEX_EFFECT_NUMBER]
	
	#get the real effect number 
	#TODO we are accessing a global here, eek!
	print "Effect num is: " + str(effectNumber)
	realEffectNumber = EffectDescriptions.quickEffectLookup[effectNumber]['realId']


	effectData = ledEffect[EffectGroup.INDEX_EFFECT_DATA]
	
	#if effect number is < 0, ignore it
	if effectNumber < 0:
		return ""

	returnStr = Utils.byte_to_hex(ledNumber) + Utils.byte_to_hex(realEffectNumber)
	
	#get the effect description
	effectDescr = EffectDescriptions.quickEffectLookup[effectNumber]
	
	#Depending on the data, time to output the values accordingly
	reqAttributes = effectDescr['reqAttributes']
	attribCount = len(reqAttributes)
	for i in range (0,attribCount):
		curAttrib = reqAttributes[i]
		attribType = curAttrib[EffectDescriptions.INDEX_TYPE]
		curData = effectData[i]
		if(attribType == EffectDescriptions.VAR_COLOR):
			returnStr += Utils.short_to_hex(curData[0])
			returnStr += Utils.short_to_hex(curData[1])
			returnStr += Utils.short_to_hex(curData[2])
		elif(attribType == EffectDescriptions.VAR_BYTE):
			returnStr += Utils.byte_to_hex(int(curData))
		elif(attribType == EffectDescriptions.VAR_WORD):
			returnStr += Utils.short_to_hex(int(curData))
		elif(attribType == EffectDescriptions.VAR_DWORD):
			returnStr += Utils.dword_to_hex(int(curData))
		elif(attribType == EffectDescriptions.VAR_HIDDEN_BYTE):
			returnStr += Utils.short_to_hex(int(curData))
			
		else:
			print "ERROR! COULD NOT DECODE EFFECT!"
	return returnStr	

		
	

