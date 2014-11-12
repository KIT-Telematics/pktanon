/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 *
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence.
 * See LICENSE file found in the top-level directory of this distribution.
 */

# ifndef PKTANON_PKTANON_H
# define PKTANON_PKTANON_H

# include <string>
# include <cstdint>

namespace pktanon
{
class Transformation;

class PktAnon
{
public:
  static void initialize ( std::string config_filename );
  static bool set_link_type ( std::uint32_t link_type ) noexcept;
  static int transform_packet ( const std::uint8_t* source_buffer,  std::uint8_t* destination_buffer, unsigned max_packet_length ) noexcept;

  static int get_erroneus_packet_length ( int packet_length );
  static const char* get_error_string ( int packet_length );

private:
  static Transformation* link_layer_transformation;
};
}

#endif // PKTANON_PKTANON_H
