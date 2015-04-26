import wx
import re
import pickle

from cfilegen import *
from globalsettings import GlobalSettings
from effectdescriptions import  EffectDescriptions
from effectgroup import EffectGroup
from myutils import Utils

class LedEditorFrame(wx.Frame):
	def __init__(self,parent,id,title):
		
		wx.Frame.__init__(self,parent,id,title,wx.DefaultPosition,(820,700))
		
		self.curFileName=""
		self.EDIT_TIME=0;
		self.BTN_TIMEADD=1;
		self.BTN_TIMEAPPLY=2;
		
		self.lastAttributeControls = []

		#holds list of effect groups
		self.effectGroups = []
		

		self.currentSelectedLed = None;
		self.currentEffectGroup = None;
		self.aknowledgeEvent = True


		#add a menubar
		menubar = wx.MenuBar()
		file = wx.Menu()
		file.Append(101,'&Open','Open file')
		file.Append(102,'&Save','Save file')
		file.Append(103,'Save &As','Save file As')
		file.Append(104,'&Export','Export song_instructions.h')
		menubar.Append(file,'&File')
		self.SetMenuBar(menubar)
		self.Bind(wx.EVT_MENU, self.loadFileEvent, id=101)
		self.Bind(wx.EVT_MENU, self.saveFileEvent, id=102)
		self.Bind(wx.EVT_MENU, self.saveFileAsEvent, id=103)
		self.Bind(wx.EVT_MENU, self.exportFileEvent, id=104)

		#add list box to hold time stamps
		self.timeStamps = ['[New Time]']
		self.timeStampListbox = wx.ListBox(self,size = (170,430),choices=self.timeStamps,style=wx.LB_SINGLE)
		self.timeStampListbox.Bind(wx.EVT_LISTBOX,self.timeGroupSelect)

		#time editbox
		self.timeEdit = wx.TextCtrl(self,id=self.EDIT_TIME, pos=(0,440), size = (130,25))
		
		#add time btn
		self.addTimeBtn = wx.Button(self,id=self.BTN_TIMEADD,pos=(0,480), size = (60,25),label="add");
		self.addTimeBtn.Bind(wx.EVT_BUTTON,self.addTimeBtnClick)

		#edit time btn
		self.editTimeBtn = wx.Button(self,id=self.BTN_TIMEAPPLY,pos=(70,480), size = (60,25),label="edit");
		self.editTimeBtn.Bind(wx.EVT_BUTTON,self.editTimeBtnClick)	
		
		#delete time btn
		self.editTimeBtn = wx.Button(self,id=self.BTN_TIMEAPPLY,pos=(0,510), size = (130,25),label="delete");
		self.editTimeBtn.Bind(wx.EVT_BUTTON,self.deleteTimeBtnClick)	
		
		#dropdown for led number selected
		ledNumbers = []
		self.ledNumbers = wx.ListBox(self,choices = ledNumbers, pos=(200,0),size=(200,275))
		self.ledNumbers.Bind(wx.EVT_LISTBOX,self.ledSelected);

		#create a panel for the effect attributes
		self.effectAttributesPanel = wx.PyScrolledWindow(self,wx.ID_ANY,pos = (450,0), size=(320,550))
		self.effectAttributesPanel.SetScrollbars(0,55,0,55);
		self.effectAttributesPanel.Hide()
		self.effectAttributesPanel.EnableScrolling(False,True);

		#dropdown of effect names
		effectNames = [];
		for effect in EffectDescriptions.Effects:
			effectNames.append(effect["name"]);
		self.effectNamesCombobox = wx.ComboBox(self.effectAttributesPanel, choices=effectNames, pos=(0,0), size =(300,25))
		self.effectNamesCombobox.Bind(wx.EVT_COMBOBOX,self.effectSelected)	
		
		'''
		#box displaying color
		self.colorPreview = wx.Panel(self,-1,wx.Point(0,300))
		self.colorPreview.SetBackgroundColour(wx.Colour(255,0,0))

		#Color scroll bar
		blueScrollBar = wx.ScrollBar(self,-1,wx.Point(100,400),wx.Size(100,100))
		blueScrollBar.SetScrollbar(wx.HORIZONTAL,0,255,255,255);
		'''

	def addTimeBtnClick(self,event):
		selectedIndex = self.timeStampListbox.GetSelection()
		if(selectedIndex == wx.NOT_FOUND):
			selectedIndex = 0;

		#get time val and make sure it's valid
		#currently only supports minutes and seconds
		timeVal = self.timeEdit.GetValue()
		if( self.VerifyTime(timeVal) == False):
			wx.MessageBox("Invalid time. Valid time ex: 52:01 001ms")
			return
		
		
		#find the appropiate spot for the time value in the list
		#splits keeps splitting time list in half, and repeats
		#For the line below we don't care about the 1st [New Entry] 
		# field
		timeStampLength = len(self.timeStamps) -1;
		insertIndex = Utils.findInsertIndex(
			self.timeStamps[1:len(self.timeStamps)],timeVal)
	
		#check if time exists already
		if(insertIndex < 0):
			wx.MessageBox("Time value already exists.")
			return
	
		self.insertTimeGroup(insertIndex,EffectGroup(GlobalSettings.LedCount,timeVal))

			
	def insertTimeGroup(self,insertIndex,timeGroupToInsert):
		#insert time into time stamps array
		headList = self.timeStamps[0:insertIndex+1]
		tailList= self.timeStamps[insertIndex+1:len(self.timeStamps)]
		self.timeStamps = headList + [timeGroupToInsert.m_time] +  tailList


		self.timeStampListbox.Insert(timeGroupToInsert.m_time,insertIndex+1)
		
		#create a new EffectGroup and tie it to the time
		headList = self.effectGroups[0:insertIndex]
		tailList = self.effectGroups[insertIndex:len(self.effectGroups)]
		self.effectGroups = headList + [timeGroupToInsert] + tailList


	#handles when a time is selected
	#TODO this gets called when window closes for
	#some reason
	def timeGroupSelect(self,event):
		selectedIndex = self.timeStampListbox.GetSelection()
		if(selectedIndex > 0):
			self.populateEffectGroupListBox(self.effectGroups[selectedIndex-1],GlobalSettings.LedCount)
			self.currentEffectGroup = self.effectGroups[selectedIndex-1]
		else:
			self.currentEffectGroup = None
		self.currentSelectedLed = None
		self.ledNumbers.SetSelection(wx.NOT_FOUND)
		self.effectAttributesPanel.Hide()
	
	#handles edit button click for time edit
	def editTimeBtnClick(self,event):

		#make sure we selected an entry, and not the first "New Entry"
		#item
		selectedIndex = self.timeStampListbox.GetSelection()
		if(selectedIndex == wx.NOT_FOUND):
			return
		elif(selectedIndex == 0):
			return
		timeVal = self.timeEdit.GetValue()
		if(self.VerifyTime(timeVal) == False):
			wx.MessageBox("Invalid time. Valid time ex: 52:01 001ms")
			return
		
		#check if the time stamp exists
		timeStampLength = len(self.timeStamps) -1;
		insertIndex = Utils.findInsertIndex(
			self.timeStamps[1:len(self.timeStamps)],timeVal)
		if(insertIndex < 0):
			wx.MessageBox("Time value already exists.")
			return
	
		#get the time group that is going to be moved
		effectGroupToKeep = self.effectGroups[selectedIndex - 1]

		self.removeTimeGroupInfo(selectedIndex-1)
		
		#get the new position to add the timestamp to 
		insertIndex = Utils.findInsertIndex(
			self.timeStamps[1:len(self.timeStamps)],timeVal)
	
		effectGroupToKeep.setTime(timeVal)
		self.insertTimeGroup(insertIndex,effectGroupToKeep)

		for i in self.effectGroups:
			print i.m_time

		print " "
	
	#called when delete time button is clicked
	def deleteTimeBtnClick(self,event):
		selectedIndex = self.timeStampListbox.GetSelection()
		if(selectedIndex <> wx.NOT_FOUND and selectedIndex > 0):
			self.removeTimeGroupInfo(selectedIndex-1)
	
	def removeTimeGroupInfo(self,effectGroupIndex):
		#remove the time group from the listing
		timeStampLength = len(self.timeStamps)
		self.timeStampListbox.Delete(effectGroupIndex+1)
		self.timeStamps = self.timeStamps[0:effectGroupIndex+1] + self.timeStamps[effectGroupIndex+2:timeStampLength]
		self.effectGroups = self.effectGroups[0:effectGroupIndex] + self.effectGroups[effectGroupIndex+1:timeStampLength-1]

	
	#Verifies time for time stamp list box is correct
	def VerifyTime(self,timeVal):
		matchVal = re.match("[0-9]{1,2}:[0-5][0-9] [0-9]{2,2}0ms",timeVal);
		if(matchVal == None): 
			return False
		return True

	#given a selected led effect group the led effect group
	#window is populated with the name of the effect for each led
	def populateEffectGroupListBox(self,effectGroup,numberOfLeds):
		
		#Calling clear triggers ledSelected
		#we got to keep track of old index
		curSelectedIndex = self.ledNumbers.GetSelection()
		self.aknowledgeEvent = False
		self.ledNumbers.Clear()
		self.aknowledgeEvent= True
		for i in range(0,numberOfLeds):
			self.ledNumbers.Append("Led " + str(i) + ": " + 
				EffectDescriptions.getEffectDescriptionById(
					effectGroup.getLedEffect(i)[EffectGroup.INDEX_EFFECT_NUMBER])["name"])
		if(curSelectedIndex <> wx.NOT_FOUND):
			self.currentSelectedLed = curSelectedIndex
		else:
			self.currentSelectedLed = None
		self.ledNumbers.SetSelection(curSelectedIndex)
	
	#called when a led number is selected
	#from the list.
	def ledSelected(self,event):
		if(self.aknowledgeEvent):
			self.setSelectedLedInfo()
			
			print "Selceted led index: " + str(self.currentSelectedLed)
			#effect drop down list to the right value
			tempIndex = EffectDescriptions.getEffectIndexById(self.currentEffectGroup.getLedEffect(self.currentSelectedLed)[EffectGroup.INDEX_EFFECT_NUMBER])
			self.effectNamesCombobox.SetSelection(tempIndex)
			self.effectSelected(None)
			self.effectAttributesPanel.Show()
	
	#sets the current selected led index for the current group
	#and sets the correct effect name selection for the led
	def setSelectedLedInfo(self):
		selectedIndex = self.ledNumbers.GetSelection()
		if selectedIndex == wx.NOT_FOUND:
			self.currentSelectedLed = None
		else:
			self.currentSelectedLed = selectedIndex

	#called when an effect is selected
	#populates gui with required components
	#for the effect
	def effectSelected(self,event):
		#Destroy the old attribute control elements
		for i in self.lastAttributeControls:
			i.Destroy();
		self.lastAttributeControls = []
		
		#get the index of the selected effect name
		selectedIndex = self.effectNamesCombobox.GetCurrentSelection()

		#get the effect number selected
		effectNum =EffectDescriptions.Effects[selectedIndex]['id']	

		#get reference to effect description
		effectDescription = EffectDescriptions.Effects[selectedIndex]

		reqAttributes = effectDescription["reqAttributes"]
		
		ledEffectInfo = self.currentEffectGroup.getLedEffect(self.currentSelectedLed)
		ledEffectData = ledEffectInfo[EffectGroup.INDEX_EFFECT_DATA]


		yPos = 50;
	
		
		attribCount = len(reqAttributes)
		hasEffectData = False
		if(ledEffectData <> None):
			effectDataLen = len(ledEffectData)
			if(effectDataLen > 0):
				#make sure that the selected effect is the same
				#effect stroed for the led
				if(effectNum == ledEffectInfo[EffectGroup.INDEX_EFFECT_NUMBER]):
					hasEffectData = True
		if(hasEffectData == False):
			self.currentEffectGroup.setLedEffect(self.currentSelectedLed,effectDescription['id'],None)

		#display controls for each attributes
		#if data is saved for led, use the data
		#TODO this does not handle if a paramter
		#is added or deleted
		for index in range (0,attribCount):
			control = None
			curAttribute = reqAttributes[index]	
			appliedEffectData = None
			if(hasEffectData == True):
				appliedEffectData = ledEffectData[index]
				

			#TODO we update the control information whenever a 
			#field is update. we can save processing time by
			#adding a commit button, or something similiar

			#Create the right control for the value that is going to be changed
			attributeType = curAttribute[EffectDescriptions.INDEX_TYPE]
			if(attributeType == EffectDescriptions.VAR_COLOR):
				#colorPicker
				control = wx.ColourPickerCtrl(self.effectAttributesPanel, pos = (150,yPos), size=(50,50))
				if (hasEffectData == True): 
					control.SetColour(appliedEffectData)
				control.Bind(wx.EVT_COLOURPICKER_CHANGED,self.effectAttribChange)
			elif( attributeType == EffectDescriptions.VAR_BYTE or
				  attributeType == EffectDescriptions.VAR_WORD or
				  attributeType == EffectDescriptions.VAR_DWORD):
				#edit box
				control = wx.TextCtrl(self.effectAttributesPanel, pos = (150,yPos), size=(100,25))
				control.Bind(wx.EVT_TEXT,self.effectAttribChange)
				if (hasEffectData == True):
					control.SetValue(appliedEffectData)	
				else:
					control.SetValue("1")
				
			#don't create a control for a hidden value
			elif(attributeType == EffectDescriptions.VAR_HIDDEN_BYTE):
				print "Hidden attrib detected, not creating controls."
				continue
			else:
				print "Unkown control type detected!"
			
			
			#We check to see if we create a contol first, if so then we
			#create a label. this is done so a label isn't created for
			# a hidden value
			#effectNameLabel
			label = wx.StaticText(self.effectAttributesPanel, label = curAttribute[EffectDescriptions.INDEX_NAME], pos = (20,yPos), size=(100,50))
		

			yPos += 40 
			
			
			self.lastAttributeControls.append(label)
			self.lastAttributeControls.append(control)
			print "last append: " + str(len(self.lastAttributeControls))
		self.populateEffectGroupListBox(self.currentEffectGroup,GlobalSettings.LedCount)
		if(hasEffectData == False):
			self.setEffectData()

	
	def effectAttribChange(self,event):
		self.setEffectData()


	#stores the effect attributes set by gui elements
	#in the proper effect object
	def setEffectData(self):
			
		#get the index of the selected effect name
		selectedIndex = self.effectNamesCombobox.GetCurrentSelection()
		
		#get reference to effect description
		effectDescription = EffectDescriptions.Effects[selectedIndex]

		reqAttributes = effectDescription["reqAttributes"]
		
		reqAttributesRange = len(reqAttributes)
		
		effectData = []

		i = 0
		controlIndex=0
		print "attribu controls count: " + str(len(self.lastAttributeControls))
		print self.lastAttributeControls
		while i < reqAttributesRange:
			
			#make sure we are not dealing with a hidden attribute
			tempAttrib = reqAttributes[i]
			attribType = tempAttrib[EffectDescriptions.INDEX_TYPE]
			if(attribType == EffectDescriptions.VAR_HIDDEN_BYTE):
				effectData.append(tempAttrib[EffectDescriptions.INDEX_HIDDEN])
				i+= 1
				continue

			#we skip the label here, get the actual control
			print "Control index is: " + str(controlIndex)
			tempControl = self.lastAttributeControls[(controlIndex*2)+1]
			if(attribType == EffectDescriptions.VAR_COLOR):
				effectData.append(tempControl.GetColour())
			elif(attribType == EffectDescriptions.VAR_BYTE or attribType == EffectDescriptions.VAR_WORD):
				effectData.append(tempControl.GetValue())
			else:
				print "Invalid effect selceted somehow!"
			i += 1
			controlIndex += 1
		self.currentEffectGroup.setLedEffect(self.currentSelectedLed,effectDescription['id'],effectData)
				

	def loadFileEvent(self,event):
		fd = wx.FileDialog(self,style=wx.FD_OPEN,defaultDir="./songs/")
		fd.ShowModal()
		fName = fd.GetPaths()[0]
		if fName <> "":
			self.curFileName = fName
			myFile = open(fName)
			self.effectGroups = pickle.load(myFile)
			myFile.close()
			#update windows
			self.timeStampListbox.Clear();
			self.timeStamps = ["[New Time]"]
			self.timeStampListbox.Append("[New Time]")
			for i in self.effectGroups:
				self.timeStampListbox.Append(i.m_time)
				self.timeStamps.append(i.m_time)

	def saveFileEvent(self,event):
		if(self.curFileName ==""):
			self.saveFileAsEvent(None)
		else:
			self.saveSongFile(self.curFileName)

	def saveFileAsEvent(self,event):
		fd = wx.FileDialog(self,style=wx.FD_SAVE|wx.FD_OVERWRITE_PROMPT,defaultDir="./songs/")
		fd.ShowModal()
		fName = fd.GetPaths()[0]
		if fName <> "":
			self.saveSongFile(fName)
			self.curFileName = fName

	def saveSongFile(self,fName):
		print "Saving file : " + fName
		myFile = open(fName,"w")
		pickle.dump(self.effectGroups,myFile)
		myFile.close()	


	def exportFileEvent(self,event):
		generateFile("../avrCode/song_instructions.c",GlobalSettings.LedCount,self.effectGroups)
		wx.MessageBox("Song exported. Please compile avr code and flash.");

class LedEditorApp(wx.App):
	def OnInit(self):
		frame = LedEditorFrame(None,-1,"Lucia - LED Animation Editor")
		frame.Centre();
		frame.Show(True)
		self.SetTopWindow(frame)
		
				
		return True

#init various classes
EffectDescriptions.init()

app = LedEditorApp(0)
app.MainLoop()


'''
import pygame
from pygame.locals import *
from LedEffectsContainer import *
from LedEffectsContainerManager import *
from appframework import *
from mybutton import *
import sys

#Play Command
class PlayCommandHandler:
	def __init__(self):
		self.paused = True;
		self.music_started = True;	

	def do(self):
		if self.music_started == True:
			if self.paused == True:
				pygame.mixer.music.unpause();
				self.paused = False;
			else:
				pygame.mixer.music.pause();
				print pygame.mixer.music.get_pos();
				self.paused = True;
		else:
			pygame.mixer.music.play();
			self.music_started = True;

class LedEditor(AppFramework):	
	led_container_rect = Rect(10,100,1000,600);
	def __init__(self):
		
		AppFramework.__init__(self,"LED Editor",1024,768)
		
		#add a sub surface for the LedEffectContainers
		self.led_effects_container_surface = self.screen.subsurface(LedEditor.led_container_rect);
		
		self.effects_manager = LedEffectsContainerManager(self.led_effects_container_surface)
		
		#Add buttons to the screen
		playButton = MyButton(20,20,50,50,PlayCommandHandler())
		self.addButton(playButton);
		playButton.draw(self.screen)
			
		
		self.effectButtonMatrix = []
		pygame.mixer.music.load("mt.mp3");
		pygame.mixer.music.rewind();
		pygame.mixer.music.play();
		pygame.mixer.music.pause();
		self.container_position = 0;	

	def handle_mouse_click(self,(x,y)):
		#pass the click to the effects manager handler
		if(LedEditor.led_container_rect.collidepoint(x,y)):
			x_pos = x - LedEditor.led_container_rect.left;
			y_pos = y - LedEditor.led_container_rect.top;
			return self.effects_manager.handle_relative_click((x_pos,y_pos));
		return 1

	def handleKeyDown(self, event):
		if event.key == K_1:
			cur_time =  pygame.mixer.music.get_pos();
			
			#round cur_time to tens place
			timeDivisionScale = 10;	
			remainderTime = cur_time % timeDivisionScale;
			if remainderTime >= 5:
				cur_time += timeDivisionScale - remainderTime
			else:
				cur_time -= remainderTime
	
			"""
			print "Cur time is: " + str(cur_time)
			curTimeCount = cur_time / timeDivisionScale;
			y = ((curTimeCount) / 700 ) * 20;
			x = (curTimeCount % 700);
			y += 300;
			print "Led 1 at " + str(curTimeCount);
			pygame.draw.rect( 
				self.screen,
				Color(0,0,0),
				Rect(x,y,5,10)
			)
			"""
			effects_group = LedEffectsContainer(cur_time);
			self.effects_manager.add_effects_container(effects_group);

#lets load a song and play it
pygame.mixer.pre_init(44100,-16,2,512) #setup mixer to avoid sound lag
pygame.init();

ledEditor = LedEditor()

done = False
while not done:
	for e in pygame.event.get():
			eventHandled = False
			if e.type is QUIT:
				done = True
			elif e.type is KEYDOWN:
				if e.key is K_ESCAPE: 
					done = True
				else:
					ledEditor.handleKeyDown(e)
			elif e.type is MOUSEBUTTONDOWN:
					ledEditor.handleMouseDown(pygame.mouse.get_pos())
	pygame.display.flip()
	pygame.time.wait(10)

#when the loop above finishes, we are quiting.
#lets list the relative times between each
#led effect container.


"""This is how the instruction is broken down
\0xff -> start of effect group packet
\0xtt\0xtt - delta time
\0xLL\0xOP[\0xDD\0XDD...] - LL = led number, op = effect number, DD = effect 
data bytes

if a led's function is just to continue it is not included
in the instruction
"""
containers = ledEditor.effects_manager.getEffectsContainers()
oldTime = 0
for item in containers:
	deltaTime = Utils.short_to_hex((item.getTime() - oldTime)/10)
	#print "Delta time is: " + str((item.getTime() - oldTime))
	ledEffectsInstructions = item.getLedEffectsInstructions()
	print "0xff," + deltaTime + ledEffectsInstructions + "0xff," 
	#print str(item.getTime() - oldTime)
	oldTime = item.getTime()
print "0x00,"
'''
