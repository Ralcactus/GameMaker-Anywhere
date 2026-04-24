.PHONY: clean 3dsx cia wuhb gamecube GameCube Gamecube NGC GCN

#3DS
3dsx cia:
	$(MAKE) -f platform/3ds/Makefile $@

#Gamecube
gamecube GameCube Gamecube NGC GCN:
	$(MAKE) -f platform/gamecube/Makefile

clean:
	$(MAKE) -f platform/3ds/Makefile clean
	$(MAKE) -f platform/wiiu/Makefile clean
	$(MAKE) -f platform/gamecube/Makefile clean
