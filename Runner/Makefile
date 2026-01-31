.PHONY: clean 3dsx cia wuhb nds

#3DS
3dsx cia:
	$(MAKE) -f platform/3ds/Makefile $@

#Wii U
wuhb:
	$(MAKE) -f platform/wiiu/Makefile $@

#DS
nds:
	$(MAKE) -f platform/nds/Makefile $@

clean:
	$(MAKE) -f platform/3ds/Makefile clean
	$(MAKE) -f platform/wiiu/Makefile clean
