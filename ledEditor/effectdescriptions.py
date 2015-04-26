class EffectDescriptions:
	VAR_COLOR = 0
	VAR_BYTE = 1
	VAR_WORD = 2
	VAR_DWORD = 3
	VAR_HIDDEN_BYTE = 4 

	INDEX_NAME = 0
	INDEX_TYPE = 1
	INDEX_HIDDEN = 2

	quickEffectLookup = {}
	quickEffectIndexLookup = {} 
	#array of effect info
	#Effect array info contains effect number, effect name:
	#then array of an array of attributes. Attribute array
	#contains effect name and type of data it is (color val, byte, etc)
	#The plan is to  populate the main window with elements based on
	#the effect type
	Effects = [ 
		{"id":-1,"realId":-1,"name":"None","reqAttributes":[]},
		{"id":1,"realId":9,"name":"Solid","reqAttributes":
			[["Color: " ,VAR_COLOR]]},
		{"id":3,"realId":3,"name":"Single Color PWM","reqAttributes":
			[["Color:",VAR_COLOR],
			 ["On Length (2ms multiplier):",VAR_WORD],
			 ["Off Length (2ms multiplier):",VAR_WORD]]},
		{"id":4,"realId":4,"name":"Fade Between 2 Colors (Solid)","reqAttributes":
			[["Begin Color", VAR_COLOR],
			 ["End Color", VAR_COLOR],
			 ["Number of 10ms steps:",VAR_WORD]]},

		{"id":5,"realId":5,"name":"Fade Between 2 Colors w/ PWM","reqAttributes":
			[["Begin Color", VAR_COLOR],
			 ["End Color", VAR_COLOR],
			 ["Number of 10ms steps:",VAR_WORD],
			 ["On Length (2ms multiplier)",VAR_WORD],
			 ["Off Length (2ms multiplier)",VAR_WORD]]},

		{"id":6,"realId":6,"name":"Fade Between 2 Colors w/ Scaling PWM","reqAttributes":
			[["Begin Color", VAR_COLOR],
			 ["End Color", VAR_COLOR],
			 ["Number of 10ms steps for color:",VAR_WORD],
			 ["Scaling Direction 0 = on to off, 1 = off to on:",VAR_WORD],
			 ["PWM 5ms Change Step Count",VAR_WORD]]},
		
		{"id":7,"realId":7,"name":"Fade Between 3 Colors w/ Scaling PWM","reqAttributes":
			[["Begin Color", VAR_COLOR],
			 ["Middle Color", VAR_COLOR],
			 ["End Color", VAR_COLOR],
			 ["Number of 10ms steps for color:",VAR_WORD],
			 ["Scaling Direction 0 = on to off, 1 = off to on:",VAR_WORD],
			 ["PWM 5ms Change Step Count",VAR_WORD]]},
		
		{"id":8,"realId":8,"name":"2 Segment Light Beam","reqAttributes":
			[[ "Number Of Beams",VAR_HIDDEN_BYTE,2],
			 ["Color 1 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 2 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD]]},

		{"id":9,"realId":8,"name":"3 Segment Light Beam","reqAttributes":
			[[ "Number Of Beams",VAR_HIDDEN_BYTE,3],
			 ["Color 1 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 2 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 3 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD]]},
		{"id":10,"realId":8,"name":"4 Segment Light Beam","reqAttributes":
			[[ "Number Of Beams",VAR_HIDDEN_BYTE,4],
			 ["Color 1 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 2 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 3 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 4 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD]]},

		{"id":11,"realId":8,"name":"6 Segment Light Beam","reqAttributes":
			[[ "Number Of Beams",VAR_HIDDEN_BYTE,6],
			 ["Color 1 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 2 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 3 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 4 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 5 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 6 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD]]},
			 
		{"id":12,"realId":8,"name":"8 Segment Light Beam","reqAttributes":
			[[ "Number Of Beams",VAR_HIDDEN_BYTE,8],
			 ["Color 1 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 2 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 3 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 4 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 5 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 6 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 7 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 8 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD]]},
			 
		{"id":13,"realId":8,"name":"10 Segment Light Beam","reqAttributes":
			[[ "Number Of Beams",VAR_HIDDEN_BYTE,10],
			 ["Color 1 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 2 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 3 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 4 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 5 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 6 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 7 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 8 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 9 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 10 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD]]},
			 
		{"id":14,"realId":8,"name":"12 Segment Light Beam","reqAttributes":
			[[ "Number Of Beams",VAR_HIDDEN_BYTE,12],
			 ["Color 1 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 2 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 3 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 4 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 5 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 6 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 7 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 8 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 9 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 10 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 11 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 12 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD]]},
			 
		{"id":15,"realId":8,"name":"14 Segment Light Beam","reqAttributes":
			[[ "Number Of Beams",VAR_HIDDEN_BYTE,14],
			 ["Color 1 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 2 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 3 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 4 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 5 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 6 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 7 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 8 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 9 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 10 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 11 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 12 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 13 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 14 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD]]},
		
		{"id":16,"realId":8,"name":"16 Segment Light Beam","reqAttributes":
			[[ "Number Of Beams",VAR_HIDDEN_BYTE,16],
			 ["Color 1 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 2 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 3 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 4 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 5 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 6 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 7 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 8 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 9 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 10 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 11 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 12 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 13 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 14 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 15 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD],
			 ["Color 16 ", VAR_COLOR],
			 ["Color length (5ms multiplier):", VAR_WORD]]},


	
	]

	@staticmethod
	def getEffectDescriptionById(aId):
		return EffectDescriptions.quickEffectLookup[aId]

	@staticmethod
	def getEffectIndexById(aId):
		return EffectDescriptions.quickEffectIndexLookup[aId]

	@staticmethod
	def init():
		effectCount = len(EffectDescriptions.Effects)
		for i in range(0,effectCount):
			tempEffect = EffectDescriptions.Effects[i]
			EffectDescriptions.quickEffectLookup[tempEffect["id"]] = tempEffect;
			EffectDescriptions.quickEffectIndexLookup[tempEffect["id"]] = i;
		
	


