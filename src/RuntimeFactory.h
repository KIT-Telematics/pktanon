/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

#ifndef PKTANON_IOFACTORY_H
#define PKTANON_IOFACTORY_H

namespace pktanon
{

class Stats;

class RecordsHandler;
class OutputSource;

class RuntimeFactory
{
public:
  static RecordsHandler* createRecordsHandler(Stats& stats);
  static OutputSource* createOutputSource();

};
}

#endif // PKTANON_IOFACTORY_H
