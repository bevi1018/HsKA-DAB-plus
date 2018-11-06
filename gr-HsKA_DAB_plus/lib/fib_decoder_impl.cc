/* -*- c++ -*- */
/* 
 * Copyright 2018 HsKA.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "fib_decoder_impl.h"

#include <HsKA_DAB_plus/FIB_Info.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		fib_decoder::sptr fib_decoder::make(char *databaseID, int32_t debug_enable)
		{
			return gnuradio::get_initial_sptr(new fib_decoder_impl(databaseID, debug_enable));
		}

		/*
		* The private constructor
		*/
		fib_decoder_impl::fib_decoder_impl(char *databaseID, int32_t debug_enable)
			: gr::sync_block("fib_decoder",
							 gr::io_signature::make(1, 1, 32*sizeof(uint8_t)),
							 gr::io_signature::make(0, 0, 0)),
			  m_database(shared_database::open_database(databaseID)),
			  m_debug_enable(debug_enable)
		{
			ServiceEnumeration *services = new ServiceEnumeration();
			m_database->set_variable<ServiceEnumeration *>(DB_SERVICE_ENUMERATION_PTR_ID, services);
		}

		/*
		* Our virtual destructor.
		*/
		fib_decoder_impl::~fib_decoder_impl()
		{
		}

		int32_t fib_decoder_impl::work(int32_t noutput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			const uint8_t *in = (const uint8_t *) input_items[0];

			for(int32_t i = 0; i < noutput_items; ++i)
				process_fib(&in[i * 32]);

			return noutput_items;
		}
	  
		void fib_decoder_impl::process_fib(const uint8_t *fib_buffer)
		{
			int32_t pos = 0;
			while(pos < FIB_DATA_FIELD_LENGTH)
			{
				if(fib_buffer[pos] == FIB_ENDMARKER)
					break;

				if(fib_buffer[pos] == FIB_PADDING)
				{
					++pos;
					continue;
				}

				int32_t type = (fib_buffer[pos] >> 5) & 0x07;
				int32_t length = fib_buffer[pos] & 0x1F;

				switch(type)
				{
					case FIB_FIG_TYPE_MCI:
						process_mci(&fib_buffer[pos + 1], length);
						break;

					case FIB_FIG_TYPE_LABEL1:
						process_label1(&fib_buffer[pos + 1], length);
						break;

					case FIB_FIG_TYPE_LABEL2:
						process_label2(&fib_buffer[pos + 1], length);
						break;

					case FIB_FIG_TYPE_FIDC:
						// Traffic Message Channel (im Standard als Reserved)
						break;

					case FIB_FIG_TYPE_CA:
						process_ca(&fib_buffer[pos + 1], length);
						break;

					default:
						break;
				}

				pos += length + 1;
			}
		}

		void fib_decoder_impl::process_mci(const uint8_t *fig_buffer, int length)
		{
			bool current_next = ((fig_buffer[0] & 0x80) != 0);						// false - current configuration; true - next configuration
			bool other_ensemble = ((fig_buffer[0] & 0x40) != 0); 					// false - this ensemble; true - other ensemble (AM, FM, DRM) 
			bool programme_data_service = ((fig_buffer[0] & 0x20) != 0);			// false - 16bit SId; true - 32bit SId
			uint8_t extension = fig_buffer[0] & 0x1F;
			
			if(other_ensemble)
				return;
			
			switch(extension)
			{
				case FIB_MCI_EXTENSION_ENSEMBLE_INFO:
					{
						if(m_debug_enable != 0)
							printf("FIB_MCI_EXTENSION_ENSEMBLE_INFO\n");
						
						EnsembleInfo ensemble_info;
						ensemble_info.country_id = (fig_buffer[1] & 0xF0) >> 4;
						ensemble_info.ensemble_reference = ((fig_buffer[1] & 0x0F) << 8) & fig_buffer[2];

						ensemble_info.change_flags = (fig_buffer[3] & 0xC0) >> 6;
						ensemble_info.alarm_flag = ((fig_buffer[3] & 0x20) != 0);		// false - alarm should not interrupt any services; true - alarm should interrupt all services

						ensemble_info.cif_count_high_part = fig_buffer[3] & 0x1F;
						ensemble_info.cif_count_low_part = fig_buffer[4];

						ensemble_info.occurrence_change = fig_buffer[5];
						
						m_database->set_variable<EnsembleInfo>(DB_ENSEMBLE_INFO_ID, ensemble_info);
					}
					break;
					
				case FIB_MCI_EXTENSION_SUBCHANNEL_ORGA:
					{
						if(m_debug_enable != 0)
							printf("FIB_MCI_EXTENSION_SUBCHANNEL_ORGA\n");
						
						int32_t processed_bytes = 0;
					
						while(processed_bytes < (length - 1))
						{	
							SubChannelOrga subchannelorga;
							subchannelorga.subchid = (fig_buffer[processed_bytes+1] & 0xFC) >> 2;
							subchannelorga.start_address = ((fig_buffer[processed_bytes+1] & 0x03 ) << 8) | fig_buffer[processed_bytes+2];
							subchannelorga.form = ((fig_buffer[processed_bytes+3] & 0x80) != 0);			// false - short form (3 bytes); true - long form (4 bytes)
							
							if(m_debug_enable != 0)
								printf("subchid: %i, start address: %i, form: %i ", subchannelorga.subchid, subchannelorga.start_address, subchannelorga.form);

							if(subchannelorga.form != true)
							{
								subchannelorga.table_switch = ((fig_buffer[processed_bytes+3] & 0x40) != 0);		// false - table 8; true - Rfu
								subchannelorga.table_index = fig_buffer[processed_bytes+3] & 0x3F;
								
								if(m_debug_enable != 0)
									printf("switch: %i, table index: %i\n", subchannelorga.table_switch, subchannelorga.table_index);
								
								processed_bytes += 3;
							}
							else
							{
								subchannelorga.option = (fig_buffer[processed_bytes+3] & 0x70) >> 4;
								subchannelorga.protection_level = (fig_buffer[processed_bytes+3] & 0x0C) >> 2;
								subchannelorga.sub_channel_size = ((fig_buffer[processed_bytes+3] & 0x03) << 8) | fig_buffer[processed_bytes+4];
								
								if(m_debug_enable != 0)
									printf("option: %i, protection level: %i, subchsize: %i\n", subchannelorga.option, subchannelorga.protection_level, subchannelorga.sub_channel_size);
								
								processed_bytes += 4;
							}
							
							m_database->set_variable<SubChannelOrga>(DB_SUBCHANNEL_ORGA_ID(subchannelorga.subchid), subchannelorga);
						}
					}
					break;
					
				case FIB_MCI_EXTENSION_SERVICE_ORGA:
					{
						if(m_debug_enable != 0)
							printf("FIB_MCI_EXTENSION_SERVICE_ORGA\n");
						
						int32_t service_components = 0;
						int32_t processed_bytes = 0;

						while(processed_bytes < length-1)
						{
							ServiceOrga serviceorga;
							
							if(!programme_data_service)
							{
								serviceorga.country_id =  (fig_buffer[processed_bytes+1] & 0xF0) >> 4;
								serviceorga.service_reference = ((fig_buffer[processed_bytes+1] & 0x0F) << 8) | fig_buffer[processed_bytes+2];
								serviceorga.service_identifier = (fig_buffer[processed_bytes+1] << 8) | fig_buffer[processed_bytes+2]; 

								serviceorga.caid = (fig_buffer[processed_bytes+3] & 0x70) >> 4;	
								serviceorga.ncomponents = fig_buffer[processed_bytes+3] & 0x0F;

								service_components = 0;	

								while(service_components < serviceorga.ncomponents)
								{
									serviceorga.tmid[service_components] = (fig_buffer[processed_bytes+4] & 0xC0) >> 6;

									if(serviceorga.tmid[service_components] != 0x03)
									{
										serviceorga.scty[service_components] = fig_buffer[processed_bytes+4] & 0x3F;
										serviceorga.subchid[service_components] = (fig_buffer[processed_bytes+5] & 0xFC) >> 2;
										serviceorga.ps[service_components] = ((fig_buffer[processed_bytes+5] & 0x02) != 0);
										serviceorga.ca_flag[service_components] = ((fig_buffer[processed_bytes+5] & 0x01) != 0);	
									}
									else
									{
										serviceorga.scid[service_components] = ((fig_buffer[processed_bytes+4] & 0x3F) << 6) | ((fig_buffer[processed_bytes+5] & 0xFC) >> 2);
										serviceorga.ps[service_components] = ((fig_buffer[processed_bytes+5] & 0x02) != 0);
										serviceorga.ca_flag[service_components] = ((fig_buffer[processed_bytes+5] & 0x01) != 0);
									}

									processed_bytes += 2;
									service_components++;	
								}
								
								processed_bytes += 3;
							}
							else
							{	
								serviceorga.ecc = fig_buffer[processed_bytes+1];
								serviceorga.country_id = (fig_buffer[processed_bytes+2] & 0xF0) >> 4;
								serviceorga.service_reference = ((fig_buffer[processed_bytes+2] & 0x0F) << 16) | (fig_buffer[processed_bytes+3] << 8) | fig_buffer[processed_bytes+4];
								serviceorga.service_identifier = (fig_buffer[processed_bytes+1] << 24) | (fig_buffer[processed_bytes+2] << 16) | (fig_buffer[processed_bytes+3] << 8) | fig_buffer[processed_bytes+4];

								serviceorga.caid = (fig_buffer[processed_bytes+5] & 0x70) >> 4;	
								serviceorga.ncomponents = fig_buffer[processed_bytes+5] & 0x0F;

								service_components = 0;

								while(service_components < serviceorga.ncomponents)
								{
									serviceorga.tmid[service_components] = (fig_buffer[processed_bytes+6] & 0xC0) >> 6;

									if(serviceorga.tmid[service_components] != 0x03)
									{
										serviceorga.scty[service_components] = fig_buffer[processed_bytes+6] & 0x3F;
										serviceorga.subchid[service_components] = (fig_buffer[processed_bytes+7] & 0xFC) >> 2;
										serviceorga.ps[service_components] = ((fig_buffer[processed_bytes+7] & 0x02) != 0);
										serviceorga.ca_flag[service_components] = ((fig_buffer[processed_bytes+7] & 0x01) != 0);	
									}
									else
									{
										serviceorga.scid[service_components] = ((fig_buffer[processed_bytes+6] & 0x3F) << 6) | ((fig_buffer[processed_bytes+7] & 0xFC) >> 2);
										serviceorga.ps[service_components] = ((fig_buffer[processed_bytes+7] & 0x02) != 0);
										serviceorga.ca_flag[service_components] = ((fig_buffer[processed_bytes+7] & 0x01) != 0);
									}

									processed_bytes += 2;
									service_components++;
								}
								
								processed_bytes += 5;
							}
							
							ServiceEnumeration *services;
							if(m_database->check_and_get_variable<ServiceEnumeration *>(DB_SERVICE_ENUMERATION_PTR_ID, &services))
								services->add_service(serviceorga.service_identifier);
							
							if(m_debug_enable != 0)
								printf("Service ID: %d\n", serviceorga.service_identifier);
							m_database->set_variable<ServiceOrga>(DB_SERVICE_ORGA_ID(serviceorga.service_identifier),serviceorga);	
						}
					}
					break;
					
				case FIB_MCI_EXTENSION_SERVICE_ORGA_PACKET_MODE:
					{
						if(m_debug_enable != 0)
							printf("FIB_MCI_EXTENSION_SERVICE_ORGA_PACKET_MODE\n");
						
						int32_t processed_bytes = 0;

						while(processed_bytes < (length - 1))
						{
							ServiceOrgaPacketMode serviceorgapacketmode;
							serviceorgapacketmode.scid = (fig_buffer[processed_bytes+1] >> 4) | ((fig_buffer[processed_bytes+2] & 0xF0) >> 4);
							serviceorgapacketmode.caorg_flag = ((fig_buffer[processed_bytes+2] & 0x01) != 0);
							serviceorgapacketmode.dg_flag = ((fig_buffer[processed_bytes+3] & 0x80) != 0);
							serviceorgapacketmode.dscty = (fig_buffer[processed_bytes+3] & 0x3F);
							serviceorgapacketmode.subchid = ((fig_buffer[processed_bytes+4] & 0xFC) >> 2);
							serviceorgapacketmode.packet_address = ((fig_buffer[processed_bytes+4] & 0x03) << 8) | fig_buffer[processed_bytes+5];
							serviceorgapacketmode.caorg = (fig_buffer[processed_bytes+6] << 8) | fig_buffer[processed_bytes+7];

							m_database->set_variable<ServiceOrgaPacketMode>(DB_SERVICE_ORGA_PACKET_MODE_ID(serviceorgapacketmode.scid), serviceorgapacketmode);	
							processed_bytes += 7;
						}
					}
					break;
					
				case FIB_MCI_EXTENSION_SERVICE_ORGA_CA:					
					{	
						if(m_debug_enable != 0)
							printf("FIB_MCI_EXTENSION_SERVICE_ORGA_CA\n");
						
						int32_t processed_bytes = 0;
				
						while(processed_bytes < (length - 1))
						{
							ServiceOrgaStreamMode serviceorgastreammode;
							serviceorgastreammode.subchid = fig_buffer[processed_bytes+1] & 0x3F;
							serviceorgastreammode.caorg = (fig_buffer[processed_bytes+2] << 8) | fig_buffer[processed_bytes+3];

							m_database->set_variable<ServiceOrgaStreamMode>(DB_SERVICE_ORGA_STREAM_MODE_ID(serviceorgastreammode.subchid), serviceorgastreammode);	
							processed_bytes += 3;
						}
					}
					break;
					
				case FIB_SI_EXTENSION_SERVICE_COMP_LANGUAGE:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_SERVICE_COMP_LANGUAGE not implemented.\n");
					break;
					
				case FIB_SI_EXTENSION_SERVICE_LINKING_INFO:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_SERVICE_LINKING_INFO not implemented.\n");
					break;
					
				case FIB_MCI_EXTENSION_CONFIG_INFO:
					if(m_debug_enable != 0)
						printf("FIB_MCI_EXTENSION_CONFIG_INFO\n");
					
					ConfigInfo configinfo;
					configinfo.services_field = (fig_buffer[1] & 0xFC) >> 2;
					configinfo.count_field = ((fig_buffer[2] & 0x03) << 8) & fig_buffer[3];
					
					m_database->set_variable<ConfigInfo>(DB_CONFIG_INFO_ID, configinfo);
					break;
					
				case FIB_MCI_EXTENSION_SERVICE_COMP_GLOBAL_DEFINITION:
					{					
						if(m_debug_enable != 0)
							printf("FIB_MCI_EXTENSION_SERVICE_COMP_GLOBAL_DEFINITION\n");

						int32_t processed_bytes = 0;

						while(processed_bytes < length-1)
						{
							ServiceGlobalDefinition serviceglobaldefinition;
							if(!programme_data_service)
							{
								serviceglobaldefinition.sid = (fig_buffer[processed_bytes+1] << 8) | fig_buffer[processed_bytes+2];
								serviceglobaldefinition.ext_flag = fig_buffer[processed_bytes+3] & 0x80;
								serviceglobaldefinition.scids = fig_buffer[processed_bytes+3] & 0x0F;
								serviceglobaldefinition.ls_flag = ((fig_buffer[processed_bytes+4] & 0x80) != 0);

								if(!serviceglobaldefinition.ls_flag)
								{
									serviceglobaldefinition.subchid = fig_buffer[processed_bytes+4] & 0x3F;
								}
								else
								{
									serviceglobaldefinition.scid = ((fig_buffer[processed_bytes+4] & 0x0F) << 8) | fig_buffer[processed_bytes+5];
								}
								
								processed_bytes += 5;
							}
							else
							{
								serviceglobaldefinition.sid = (fig_buffer[processed_bytes+1] << 24) | (fig_buffer[processed_bytes+2] << 16) | (fig_buffer[processed_bytes+3] << 8)| fig_buffer[processed_bytes+4];
								serviceglobaldefinition.ext_flag = fig_buffer[processed_bytes+5] & 0x80;
								serviceglobaldefinition.scids = fig_buffer[processed_bytes+5] & 0x0F;
								serviceglobaldefinition.ls_flag = ((fig_buffer[processed_bytes+6] & 0x80) != 0);

								if(!serviceglobaldefinition.ls_flag)
								{
									serviceglobaldefinition.subchid = fig_buffer[processed_bytes+6] & 0x3F;
								}
								else
								{
									serviceglobaldefinition.scid = ((fig_buffer[processed_bytes+6] & 0x0F) << 8) | fig_buffer[processed_bytes+7];
								}
								
								processed_bytes += 7;
							}
							
							m_database->set_variable<ServiceGlobalDefinition>(DB_SERVICE_COMP_GLOBAL_DEFINITION_ID(serviceglobaldefinition.sid), serviceglobaldefinition);
						}
					}
					break;
					
				case FIB_SI_EXTENSION_COUNTRY_LTO:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_COUNTRY_LTO not implemented.\n");
					break;
					
				case FIB_SI_EXTENSION_TIME_AND_COUNTRYID:
					if(m_debug_enable != 0)
						printf("FIB_SI_EXTENSION_TIME_AND_COUNTRYID\n");
					
					DateandTime dateandtime;
					dateandtime.mjd = ((fig_buffer[1] & 0x7f) << 10) | (fig_buffer[2] << 2) | ((fig_buffer[3] & 0xc0) >> 6);
					dateandtime.lsi = ((fig_buffer[3] & 0x40) != 0);
					dateandtime.utc_flag = ((fig_buffer[3] & 0x08) != 0);
					if(dateandtime.utc_flag)
					{
						dateandtime.hours = ((fig_buffer[3] & 0x07) << 2) | ((fig_buffer[4] & 0xc0) >> 6);
						dateandtime.minutes = (fig_buffer[4] & 0x3f);
						dateandtime.seconds = ((fig_buffer[5] & 0xfc) >> 2);
						dateandtime.milliseconds = ((fig_buffer[5] & 0x03) << 8) | fig_buffer[6];	
					}
					else
					{
						dateandtime.hours = ((fig_buffer[3] & 0x07) << 2) | ((fig_buffer[4] & 0xc0) >> 6);
						dateandtime.minutes = (fig_buffer[4] & 0x3f);	
					}
					
					m_database->set_variable<DateandTime>(DB_DATE_AND_TIME_ID,dateandtime);
					break;
					
				case FIB_SI_EXTENSION_USER_APPLICATION_INFO:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_USER_APPLICATION_INFO not implemented.\n");
					break;
					
				case FIB_MCI_EXTENSION_SUBCHANNEL_PACKET_MODE_FEC:
					{
						if(m_debug_enable != 0)
							printf("FIB_MCI_EXTENSION_SUBCHANNEL_PACKET_MODE_FEC\n");
						
						int32_t processed_bytes = 0;

						while(processed_bytes < length-1)
						{	
							FECSubChannelOrga fecsubchannelorga;
							fecsubchannelorga.subchid = (fig_buffer[processed_bytes+1] & 0xFC) >> 2;
							fecsubchannelorga.fec_scheme = fig_buffer[processed_bytes+1] & 0x03;

							if(m_debug_enable != 0)
								printf("subchid: %i, fec: %i\n",fecsubchannelorga.subchid, fecsubchannelorga.fec_scheme);

							m_database->set_variable<FECSubChannelOrga>(DB_FEC_SUBCH_ORGA_ID(fecsubchannelorga.subchid),fecsubchannelorga);		
							processed_bytes++;	
						}
					}
					break;
					
				case FIB_SI_EXTENSION_PROGRAMME_TYPE:
					{
						if(m_debug_enable != 0)
							printf("FIB_SI_EXTENSION_PROGRAMME_TYPE\n");
						
						int32_t processed_bytes = 0;

						while(processed_bytes < length-1)
						{
							ProgrammeType programmetype;
							programmetype.sid = (fig_buffer[processed_bytes+1] << 8) | fig_buffer[processed_bytes+2];
							programmetype.sd = ((fig_buffer[processed_bytes+3] & 0x80) != 0);
							programmetype.international_code = fig_buffer[processed_bytes+4] & 0x1F;

							if(m_debug_enable != 0)
								printf("sid: %i, programme type: %s\n",programmetype.sid,programmetypecode[programmetype.international_code]);

							m_database->set_variable<ProgrammeType>(DB_PROGRAM_TYPE_ID(programmetype.sid),programmetype);	
							processed_bytes += 4;
						}
					}
					break;
					
				case FIB_SI_EXTENSION_ANNOUNCEMENT_SUPPORT:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_ANNOUNCEMENT_SUPPORT not implemented.\n");
					break;
					
				case FIB_SI_EXTENSION_ANNOUNCEMENT_SWITCHING:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_ANNOUNCEMENT_SWITCHING not implemented.\n");
					break;
					
				case FIB_SI_EXTENSION_SERVICE_COMP_INFO:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_SERVICE_COMP_INFO not implemented.\n");
					break;
					
				case FIB_SI_EXTENSION_FREQUENCY_INFO:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_FREQUENCY_INFO not implemented.\n");
					break;
					
				case FIB_SI_EXTENSION_OE_SERVICES:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_OE_SERVICES not implemented.\n");
					break;
					
				case FIB_SI_EXTENSION_OE_ANNOUNCEMENT_SUPPORT:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_OE_ANNOUNCEMENT_SUPPORT not implemented.\n");
					break;
					
				case FIB_SI_EXTENSION_OE_ANNOUNCEMENT_SWITCHING:
					if(m_debug_enable >= 2)
						printf("FIB_SI_EXTENSION_OE_ANNOUNCEMENT_SWITCHING not implemented.\n");
					break;
					
				default:
					break;
			}
		}
		
		void fib_decoder_impl::process_label1(const uint8_t *fig_buffer, int length)
		{
			uint8_t charset = (fig_buffer[0] & 0xF0) >> 4;
			uint8_t extension = fig_buffer[0] & 0x0F;
			
			switch(extension)
			{
				case FIB_SI_EXTENSION_ENSEMBLE_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("FIB_SI_EXTENSION_ENSEMBLE_LABEL\n");

						EnsembleLabel ensemblelabel;
						ensemblelabel.ensemble_identifier = (fig_buffer[1] << 8) | fig_buffer[2];

						for(int32_t i = 0; i < 16; ++i)
							ensemblelabel.character_field[i] = fig_buffer[i+3];
						ensemblelabel.character_field[16] = 0;

						if(m_debug_enable != 0)	
							printf("ensemble: %s\n",ensemblelabel.character_field);

						ensemblelabel.character_flag_field = (fig_buffer[19] << 8) | fig_buffer[20];
						m_database->set_variable<EnsembleLabel>(DB_ENSEMBLE_LABEL_ID, ensemblelabel);
					}
					break;			
				case FIB_SI_EXTENSION_PROGRAMME_SERVICE_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("FIB_SI_EXTENSION_PROGRAMME_SERVICE_LABEL\n");

						ProgrammeServiceLabel programmeservicelabel;
						programmeservicelabel.service_identifier = (fig_buffer[1] << 8) | fig_buffer[2];

						for(int32_t i = 0; i < 16; ++i)
							programmeservicelabel.character_field[i] = fig_buffer[i+3];	
						programmeservicelabel.character_field[16] = 0;

						if(m_debug_enable != 0)	
							printf("service (sid: %d): %s \n",programmeservicelabel.service_identifier, programmeservicelabel.character_field);

						programmeservicelabel.character_flag_field = (fig_buffer[19] << 8) | fig_buffer[20];
						m_database->set_variable<ProgrammeServiceLabel>(DB_PROGRAM_SERVICE_LABEL_ID(programmeservicelabel.service_identifier), programmeservicelabel);
					}
					break;
				case FIB_SI_EXTENSION_SERVICE_COMP_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("FIB_SI_EXTENSION_SERVICE_COMP_LABEL\n");

						ServiceComponentLabel servicecomponentlabel;
						servicecomponentlabel.pd = ((fig_buffer[1] & 0x80) != 0);
						servicecomponentlabel.scids = fig_buffer[1] & 0x0F;

						if(servicecomponentlabel.pd)
							servicecomponentlabel.service_identifier = (fig_buffer[2] << 24) | (fig_buffer[3] << 16) | (fig_buffer[4] << 8) | fig_buffer[5];
						else
							servicecomponentlabel.service_identifier = (fig_buffer[2] << 8) | fig_buffer[3];

						m_database->set_variable<ServiceComponentLabel>(DB_SERVICE_COMP_LABEL_ID(servicecomponentlabel.service_identifier), servicecomponentlabel);
					}
					break;
				case FIB_SI_EXTENSION_DATA_SERVICE_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("FIB_SI_EXTENSION_DATA_SERVICE_LABEL\n");

						DataServiceLabel dataservicelabel;
						dataservicelabel.service_identifier = (fig_buffer[1] << 24) | (fig_buffer[2] << 16) | (fig_buffer[3] << 8) | fig_buffer[4];

						for(int i = 0; i < 16; ++i)
							dataservicelabel.character_field[i] = fig_buffer[i+5];
						dataservicelabel.character_field[16] = 0;

						if(m_debug_enable != 0)	
							printf("service: %s \n",dataservicelabel.character_field);

						dataservicelabel.character_flag_field = (fig_buffer[21] << 8) | fig_buffer[22];

						m_database->set_variable<DataServiceLabel>(DB_DATA_SERVICE_LABEL_ID(dataservicelabel.service_identifier), dataservicelabel);
					}
					break;
				case FIB_SI_EXTENSION_XPAD_USER_APPLICATION_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("FIB_SI_EXTENSION_XPAD_USER_APPLICATION_LABEL\n");

						XPadUserApplicationLabel xpaduserapplicationlabel;
						xpaduserapplicationlabel.pd = ((fig_buffer[1] & 0x80) != 0);
						xpaduserapplicationlabel.scids = fig_buffer[1] & 0x0F;
						if(xpaduserapplicationlabel.pd)
						{
							xpaduserapplicationlabel.service_identifier = (fig_buffer[2] << 24) | (fig_buffer[3] << 16) | (fig_buffer[4] << 8) | fig_buffer[5];
							xpaduserapplicationlabel.xpad_application_type = (fig_buffer[6] & 0x1f);						
						}
						else
						{
							xpaduserapplicationlabel.service_identifier = (fig_buffer[2] << 8) | fig_buffer[3];
							xpaduserapplicationlabel.xpad_application_type = (fig_buffer[4] & 0x1f);
						}

						m_database->set_variable<XPadUserApplicationLabel>(DB_XPAD_USER_APPLICATION_LABEL_ID(xpaduserapplicationlabel.service_identifier), xpaduserapplicationlabel);
					}
					break;
				default:
					break;
			}
		}
		
		void fib_decoder_impl::process_label2(const uint8_t *fig_buffer, int length)
		{
			bool toggle_flag = ((fig_buffer[0] & 0x80) != 0);
			uint8_t segment_index = (fig_buffer[0] & 0x70) >> 4;
			uint8_t extension = fig_buffer[0] & 0x0F;
			
			switch(extension)
			{
				case FIB_SI_EXTENSION_ENSEMBLE_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("Type 2 FIB_SI_EXTENSION_ENSEMBLE_LABEL\n");

						EnsembleLabel ensemblelabel;
						ensemblelabel.ensemble_identifier = (fig_buffer[1] << 8) | fig_buffer[2];

						for(int32_t i = 0; i < 16; ++i)
							ensemblelabel.character_field[i] = fig_buffer[i+3];

						if(m_debug_enable != 0)	
							printf("ensemble: %s \n",ensemblelabel.character_field);

						ensemblelabel.character_flag_field = (fig_buffer[19] << 8) | fig_buffer[20];
						m_database->set_variable<EnsembleLabel>(DB_ENSEMBLE_LABEL2_ID, ensemblelabel);
					}
					break;			
				case FIB_SI_EXTENSION_PROGRAMME_SERVICE_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("Type 2 FIB_SI_EXTENSION_PROGRAMME_SERVICE_LABEL\n");

						ProgrammeServiceLabel programmeservicelabel;
						programmeservicelabel.service_identifier = (fig_buffer[1] << 8) | fig_buffer[2];

						for(int32_t i = 0; i < 16; ++i)
							programmeservicelabel.character_field[i] = fig_buffer[i+3];	

						if(m_debug_enable != 0)	
							printf("service (sid: %d): %s \n",programmeservicelabel.service_identifier, programmeservicelabel.character_field);

						programmeservicelabel.character_flag_field = (fig_buffer[19] << 8) | fig_buffer[20];
						m_database->set_variable<ProgrammeServiceLabel>(DB_PROGRAM_SERVICE_LABEL2_ID(programmeservicelabel.service_identifier), programmeservicelabel);
					}
					break;
				case FIB_SI_EXTENSION_SERVICE_COMP_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("Type 2 FIB_SI_EXTENSION_SERVICE_COMP_LABEL\n");

						ServiceComponentLabel servicecomponentlabel;
						servicecomponentlabel.pd = ((fig_buffer[1] & 0x80) != 0);
						servicecomponentlabel.scids = fig_buffer[1] & 0x0F;

						if(servicecomponentlabel.pd)
							servicecomponentlabel.service_identifier = (fig_buffer[2] << 24) | (fig_buffer[3] << 16) | (fig_buffer[4] << 8) | fig_buffer[5];
						else
							servicecomponentlabel.service_identifier = (fig_buffer[2] << 8) | fig_buffer[3];

						m_database->set_variable<ServiceComponentLabel>(DB_SERVICE_COMP_LABEL2_ID(servicecomponentlabel.service_identifier), servicecomponentlabel);
					}
					break;
				case FIB_SI_EXTENSION_DATA_SERVICE_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("Type 2 FIB_SI_EXTENSION_DATA_SERVICE_LABEL\n");

						DataServiceLabel dataservicelabel;
						dataservicelabel.service_identifier = (fig_buffer[1] << 24) | (fig_buffer[2] << 16) | (fig_buffer[3] << 8) | fig_buffer[4];

						for(int i = 0; i < 16; ++i)
							dataservicelabel.character_field[i] = fig_buffer[i+5];

						if(m_debug_enable != 0)	
							printf("service: %s \n",dataservicelabel.character_field);

						dataservicelabel.character_flag_field = (fig_buffer[21] << 8) | fig_buffer[22];

						m_database->set_variable<DataServiceLabel>(DB_DATA_SERVICE_LABEL2_ID(dataservicelabel.service_identifier), dataservicelabel);
					}
					break;
				case FIB_SI_EXTENSION_XPAD_USER_APPLICATION_LABEL:
					{
						if(m_debug_enable != 0)	
							printf("Type 2 FIB_SI_EXTENSION_XPAD_USER_APPLICATION_LABEL\n");

						XPadUserApplicationLabel xpaduserapplicationlabel;
						xpaduserapplicationlabel.pd = ((fig_buffer[1] & 0x80) != 0);
						xpaduserapplicationlabel.scids = fig_buffer[1] & 0x0F;
						if(xpaduserapplicationlabel.pd)
						{
							xpaduserapplicationlabel.service_identifier = (fig_buffer[2] << 24) | (fig_buffer[3] << 16) | (fig_buffer[4] << 8) | fig_buffer[5];
							xpaduserapplicationlabel.xpad_application_type = (fig_buffer[6] & 0x1f);						
						}
						else
						{
							xpaduserapplicationlabel.service_identifier = (fig_buffer[2] << 8) | fig_buffer[3];
							xpaduserapplicationlabel.xpad_application_type = (fig_buffer[4] & 0x1f);
						}

						m_database->set_variable<XPadUserApplicationLabel>(DB_XPAD_USER_APPLICATION_LABEL2_ID(xpaduserapplicationlabel.service_identifier), xpaduserapplicationlabel);
					}
					break;
				default:
					break;
			}			
		}
		
		void fib_decoder_impl::process_ca(const uint8_t *fig_buffer, int length)
		{
		}
  } /* namespace HsKA_DAB_plus */
} /* namespace gr */

