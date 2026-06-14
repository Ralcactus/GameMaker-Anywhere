.PHONY: clean 3dsx cia wuhb gamecube GameCube Gamecube NGC GCN wii Wii WII

#3DS
3dsx cia:
	$(MAKE) -f platform/3ds/makefile $@

#Gamecube
gamecube GameCube Gamecube NGC GCN:
	$(MAKE) -f platform/gamecube/makefile

#Wii
wii Wii WII:
	$(MAKE) -f platform/wii/makefile

clean:
	$(MAKE) -f platform/3ds/makefile clean
	$(MAKE) -f platform/wiiu/makefile clean
	$(MAKE) -f platform/gamecube/makefile clean
	$(MAKE) -f platform/wii/makefile clean
