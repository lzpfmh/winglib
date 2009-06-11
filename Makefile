
# regular apps
BUILDDIRS := app/hello lib/oexlib app/test app/iptest app/srvrund app/srvmod app/modrun

#-------------------------------------------------------------------
# BUILDDEP	= Build dependent libraries
# SQMOD 	= Squirrel
# 3D 		= Irrlicht
# GUI 		= OpenGL / wxWidgets
# GUI_ES	= Embedded OpenGL / wxWidgets 
#-------------------------------------------------------------------

ifdef 3D
	BUILDDIRS := $(BUILDDIRS) app/irr_test
endif

ifdef SQMOD
	BUILDDIRS := $(BUILDDIRS) lib/sqbind app/sqrl app/sqengine \
							  app/sq_modules/sqmod_test app/sq_modules/sqmod_cell \
	                          app/sq_modules/sqmod_http app/sq_modules/sqmod_gdchart \
	                          app/sq_modules/sqmod_curl app/sq_modules/sqmod_webkit
	ifdef 3D
		BUILDDIRS := $(BUILDDIRS) app/sq_modules/sqmod_irrlicht
	endif
	                          
endif

ifdef GUI
	BUILDDIRS := $(BUILDDIRS) app/vidcap app/avi_scan
	ifdef 3D
		BUILDDIRS := $(BUILDDIRS) app/irr_wxtest
	endif
	ifdef BUILD_WEBKIT
		BUILDDIRS := $(BUILDDIRS) app/wx_webkit
	endif
endif

ifdef BUILDDEP
	ifdef GUI
		BUILDDIRS := dep/gui $(BUILDDIRS)
	endif	
	ifdef GUI_ES
		BUILDDIRS := dep/gui_es $(BUILDDIRS)
	endif	
	BUILDDIRS := dep $(BUILDDIRS)
endif

.PHONY all: $(BUILDDIRS)

.PHONY $(BUILDDIRS) :
	$(MAKE) -C $@ $(BUILDTYPE)

