idf_component_register(SRCS  
                            "main.c" 
                            "pinInit/pinInit.c"

                    INCLUDE_DIRS 
                            "."
                            "pinInit"
                    EMBED_TXTFILES
                        
                    )

fatfs_create_rawflash_image(storage ../site/dist FLASH_IN_PROJECT)
