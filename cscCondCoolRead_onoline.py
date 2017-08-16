###############################################################
# author Caleb Lampen lampen@physics.arizona.edu 
#        You   Zhou   youzhou@email.arizona.edu
#==============================================================
# cscCondCoolRead.py # generates a calibration file from the data in database
# also meant to be a bit of an example joboptions file of the settings that must be set

#Set values for various parameters

if("doOnline" not in dir()):
  doOnline = True

#Set which parameters to read from SQLite file
if('readPars' not in dir()):
  readPars = ["status"]
  #we dont have pslope here because we almost never change it

if('readDb' not in dir()):
#  readDb = "COMP200"
  readDb = "CONDBR2"
#  readDb = "OFLP200"

if('output' not in dir()):
  output = 'mycool.2017.07.26.status'

#Run number to read from COOL
#Set to -1 to not set
if("forceRunNumber" not in dir()):
  #forceRunNumber = 2147483647 #Set very high so that is at end of last IOV
  forceRunNumber = 330470
  #You can only retrieve data of one perticular run number

if('readFromLocalFile' not in dir()):
  readFromLocalFile = False

#Need to override to tags used in SQLite file, as we can't count on Global tag to 
#Resolve correctly
if('f001Override' not in dir()):
  f001Override = "CscFthold-BLK-UPD1-003-00"
if('noiseOverride' not in dir()):
  noiseOverride = "CscNoise-BLK-UPD1-003-00"
if('pedOverride' not in dir()):
  pedOverride = "CscPed-BLK-UPD1-003-00"
if('statusOverride' not in dir()):
  statusOverride = "CscStatOnl-RUN2-HLT-UPD1-002-00"
if('rmsOverride' not in dir()):
  rmsOverride = "CscRms-BLK-UPD1-003-00"
if('t0baseOverride' not in dir()):
  t0baseOverride = "CscT0base-BLK-UPD1-000-02"
if('t0phaseOverride' not in dir()):
  t0phaseOverride = "CscT0phase-BLK-UPD2-001-00"

if('forceReadAsChannelCategory' not in dir()):
  forceReadAsChannelCategory = False


from AthenaCommon.AppMgr import theApp
theApp.EvtMax =1 

from AthenaCommon.AlgSequence import AlgSequence
topSequence = AlgSequence()

#CscReadWriteCoolStr to read a file from database
from MuonCondCool.MuonCondCoolConf import MuonCalib__CscReadWriteCoolStr
topSequence += MuonCalib__CscReadWriteCoolStr()
CscReadWriteCoolStr = MuonCalib__CscReadWriteCoolStr()
CscReadWriteCoolStr.Write=False
#Output stuff 
CscReadWriteCoolStr.Read = True
CscReadWriteCoolStr.oFile = output
#01-00 is the "online" filetype using online ids. 00-00 is the offline datatype that comes from the
#CscDoCalib algorithm
#CscReadWriteCoolStr.OutFileType ="04-01"
CscReadWriteCoolStr.OutFileType ="04-00"
CscReadWriteCoolStr.OutParameters = readPars
if(forceReadAsChannelCategory):
  CscReadWriteCoolStr.ForceReadAsChannelCategory = True

#---Set detector description tag
#DetDescrVersion = "ATLAS-GEONF-08-00-00"
#this was abandoned temporarily

#----The next block will not have to be reproduced in some 
#----jobs. Top level joboptions like RecExCommon_topOptions.py
#----and RecExCommissionFlags_jobOptions.py usually set the appropriate flags.
#>>from AthenaCommon.GlobalFlags import GlobalFlags #GlobalFlags
#>>GlobalFlags.DetGeo.set_atlas() 	
#>>if(readDb == "OFLP200"):
#>>  GlobalFlags.DataSource.set_geant4()
#>>elif(readDb == "COMP200"): 
#>>  GlobalFlags.DataSource.set_data();
#>>else:
#>>  print "ERROR! readDb = " + readDb + " is invalid! Must be either COMP200 or OFLP200!"

#from AthenaCommon.GlobalFlags import globalflags #GlobalFlags
#globalflags.DetGeo = 'atlas' 
#if(readDb == "OFLP200"):
#  globalflags.DataSource = 'geant4'
#elif(readDb == "COMP200"): 
#  globalflags.DataSource = 'data'
#else:
#  print "ERROR! readDb = " + readDb + " is invalid! Must be either COMP200 or OFLP200!"

from AthenaCommon.GlobalFlags import globalflags as GlobalFlags
GlobalFlags.DetGeo.set_Value_and_Lock('atlas')
if(readDb == "OFLP200"):
  GlobalFlags.DataSource.set_Value_and_Lock('geant4')
elif(readDb == "COMP200"):
  GlobalFlags.DataSource.set_Value_and_Lock('data')
  GlobalFlags.DatabaseInstance.set_Value_and_Lock('COMP200')
elif(readDb == "CONDBR2"):
  GlobalFlags.DataSource.set_Value_and_Lock('data')
  GlobalFlags.DatabaseInstance.set_Value_and_Lock('CONDBR2')
else:
  print "ERROR! readDb = " + readDb + " is invalid! Must be either COMP200 or OFLP200 or CONDBR2!"

from AthenaCommon.AthenaCommonFlags import athenaCommonFlags
athenaCommonFlags.isOnline.set_Value_and_Lock(True)

from IOVDbSvc.CondDB import conddb

#include ("RecExCond/RecExCommon_flags.py")
#DetFlags.ID_setOff()  #speeds up and prevents problems in commissioning database
#DetFlags.Calo_setOff()
#include ("RecExCond/AllDet_detDescr.py")
# stole from MDT copy_to_cool_configurator.py
from AthenaCommon.DetFlags import DetFlags
DetFlags.detdescr.Muon_setOn()
DetFlags.detdescr.ID_setOff()
DetFlags.detdescr.Calo_setOff()

from AtlasGeoModel import SetGeometryVersion, GeoModelInit
GeoModelSvc = ServiceMgr.GeoModelSvc
if  "atlasVersion" in dir():
    GeoModelSvc.AtlasVersion = atlasVersion

#Set up IOVDbService
#Pick a database tag. Should correspond with DetDescrVersion. See AtlasGeomDBTags page on twiki
#conddb.setGlobalTag('COMCOND-BLKPA-RUN1-05')
conddb.setGlobalTag('CONDBR2-HLTP-2015-07')
#conddb.setGlobalTag('OFLCOND-RUN12-SDR-06')


if(forceRunNumber > 0):
  Service("IOVDbSvc").forceRunNumber = forceRunNumber;
  Service("IOVDbSvc").forceLumiblockNumber = 1

from MuonCondSvc.CscCondDB import cscCondDB
#Read from local file. Must use before "addPedFolders"
#cscCondDB.setFolderSuffix("<dbConnection>sqlite://;schema=mod.db;dbname=COMP200</dbConnection>")

if(readFromLocalFile):
  cscCondDB.useLocalFile()

from MuonCondSvc.CscCoolFolders import dict as coolFolders

#Set pedestals
#Note if override strings are "", no overide will occur
if("ped" in readPars):
  cscCondDB.addPedFolder(pedOverride)
if("noise" in readPars):
  cscCondDB.addNoiseFolder(noiseOverride)
if("rms" in readPars):
  cscCondDB.addRmsFolder(rmsOverride)
if("f001" in readPars):
  cscCondDB.addFtholdFolder(f001Override)
if("status" in readPars):
  cscCondDB.addStatusFolder(statusOverride)
if("t0phase" in readPars):
  cscCondDB.addT0PhaseFolder(t0phaseOverride)
if("t0base" in readPars):
  cscCondDB.addT0BaseFolder(t0baseOverride)

#--------------------------------------------------------------
# Set output level threshold (1=VERBOSE, 2=DEBUG, 3=INFO, 4=WARNING, 5=ERROR, 6=FATAL )
#--------------------------------------------------------------
#import AthenaPoolCnvSvc.ReadAthenaPool
#ServiceMgr.MessageSvc.OutputLevel      = VERBOSE
#IOVDbSvc = Service( "IOVDbSvc" )
#IOVDbSvc.OutputLevel        = 2
#ServiceMgr.EventSelector.RunNumber		= 0
#ServiceMgr.EventSelector.FirstEvent	= 0
