.PHONY : create_iso
create_iso:
	# @hdiutil makehybrid -iso -joliet -hfs -default-volume-name BoOS -hard-disk-boot -hfs-blessed-directory /efi/boot -ov -o $(OUTROOT)/boos $(SYSROOT_OUT)
	@hdiutil create -srcfolder $(SYSROOT_OUT) -fs MS-DOS -fsargs "-F 32" -format UDRW -layout GPTSPUD -size 40m -volname BoOS -ov $(OUTROOT)/boos
	@mv $(OUTROOT)/boos.dmg $(OUTROOT)/boos.iso


# @hdiutil makehybrid -iso -joliet -hfs -default-volume-name BoOS -hard-disk-boot -ov -o $(OUTROOT)/boos $(SYSROOT_OUT)
# @hdiutil create -srcfolder $(SYSROOT_OUT) -fs MS-DOS -fsargs "-F 32" -format UDRW -layout GPTSPUD -size 40m -volname BoOS -ov $(OUTROOT)/boos
