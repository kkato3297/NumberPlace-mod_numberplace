mod_numberplace.la: mod_numberplace.slo NumberPlace.slo contents.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_numberplace.lo NumberPlace.lo contents.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_numberplace.la
