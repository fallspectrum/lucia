import re
class EffectGroup:
	INDEX_EFFECT_NUMBER = 0
	INDEX_EFFECT_DATA = 1
	
	def __init__(self,totalLeds,time):
		self.m_time = time
		self.m_effects = {}
		for i in range (0,totalLeds):
			self.setLedEffect(i,-1,None)

	#set time in text format, ex 00:01 010ms
	def setTime(self,time):
		self.m_time = time

	def setLedEffect(self,ledNumber,effectNumber,effectData):
		self.m_effects[ledNumber] =  [effectNumber,effectData]

	def getLedEffect(self,ledNumber) :
		return self.m_effects[ledNumber]

	#holds the time value. Should be formatted
	#as 01:00 200ms
	def getTime(self):
		return self.m_time
	
	def getTimeAs10msCount(self):
		m = re.search('([0-9]{2,2}):([0-9]{2,2}) ([0-9]{3,3})ms',self.m_time)
		minMili = 60000 *int( m.group(1))
		secondMili = 1000 * int(m.group(2))
		miliMili = int(m.group(3))
		
		totalMili = minMili +secondMili +miliMili
		miliAs10Step = totalMili / 10
		return miliAs10Step
