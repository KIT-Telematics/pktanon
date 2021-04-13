/**
 * Copyright (c) 2014, Institute of Telematics, Karlsruhe Institute of Technology.
 * 
 * This file is part of the PktAnon project. PktAnon is distributed under 2-clause BSD licence. 
 * See LICENSE file found in the top-level directory of this distribution.
 */

# ifndef PKTANON_PARAMS_H
# define PKTANON_PARAMS_H

# include <stdexcept>
# include <string>
# include <unordered_map>

namespace pktanon
{

enum class TFD_VAL {TRUE, FALSE, DEFAULT};
struct ID_VAL
{
  ID_VAL() : int_val(0), is_default(true) {}
  ID_VAL(unsigned long value) : int_val(value), is_default(false) {}

  unsigned long int_val;
  bool is_default;
};

# define THROW_MISSING_VAL(key) throw std::runtime_error(std::string("missing parameter: ") +  key);
# define THROW_WRONG_VAL(key, wrong_value) throw std::runtime_error("illegal value for boolean parameter " + key + ": " + wrong_value);

/**
 * wrapper arount hashtable (inspired by java.lang.Properties)
 */
class Params
{
public:
  ~Params() = default;
  Params() :props() {};

//   Params(Params&& other) : props(std::move(other.props)) {};

  bool has_param(const std::string& key) const { return props.find(key) !=  props.end(); }

  const std::string& get_param(const std::string& key) const
  {
    if (!has_param(key)) throw std::runtime_error(std::string("missing parameter: ") +  key);

    return (*props.find(key)).second;
  }

  const std::string& get_param(const std::string& key,  const std::string& default_value) const
  {
    if (has_param(key)) return (*props.find(key)).second;

    return default_value;
  }

  bool get_bool_param(const std::string& key) const
  {
    if (!has_param(key)) throw std::runtime_error(std::string("missing parameter: ") +  key);

    const auto& str_val = (*props.find(key)).second;

    if (is_true_value(str_val)) { return true; }
    else if (is_false_value(str_val)) { return false; }
    else { THROW_WRONG_VAL(key, str_val); }
  }

  bool get_bool_param(const std::string& key,  bool default_value) const
  {
    if (has_param(key)) { return get_bool_param(key); }

    return default_value;
  }

  ///param with values - true/false/default
  TFD_VAL get_tfd_param(const std::string& key) const
  {
    if (!has_param(key)) return TFD_VAL::DEFAULT;

    const auto& str_val = (*props.find(key)).second;
    if (is_default_value(str_val)) {return TFD_VAL::DEFAULT;}
    else if (is_true_value(str_val)) {return TFD_VAL::TRUE;}
    else if (is_false_value(str_val)) {return TFD_VAL::FALSE;}
    else {THROW_WRONG_VAL(key, str_val);}
  }

  unsigned long get_uint_param(const std::string& key) const
  {
    if (!has_param(key)) throw std::runtime_error(std::string("missing parameter: ") +  key);

    const auto& str_val = (*props.find(key)).second;
    return std::stoul(str_val);
  }

  unsigned long get_uint_param(const std::string& key,  unsigned long default_value) const
  {
    if (has_param(key)) { return get_uint_param(key); }

    return default_value;
  }

  ID_VAL get_uintdefault_param(const std::string& key) const
  {
    if (!has_param(key)) return ID_VAL();

    const auto& str_val = (*props.find(key)).second;
    if (is_default_value(str_val)) {return ID_VAL();}
    else { return ID_VAL(std::stoul(str_val)); }
  }

  bool is_missing_or_has_default_value(const std::string& key) const
  {
    if (!has_param(key)) return true;
      
    const auto& str_val = (*props.find(key)).second;
    if (is_default_value(str_val)) return true;

    return false;
  }

  void add_param(const std::string&& key,  const std::string&& value)
  {
    props.emplace(std::move(key),  std::move(value));
  }

private:
  bool is_default_value(const std::string& value) const { return value.compare("auto") ==  0  || value.compare("default") ==  0;}
  bool is_true_value(const std::string& value) const { return value.compare("yes") == 0  || value.compare("on") ==  0  || value.compare("1") ==  0;}
  bool is_false_value(const std::string& value) const { return value.compare("no") == 0  || value.compare("off") ==  0  || value.compare("0") ==  0;}

  std::unordered_map<std::string, std::string> props;
};

}

# endif
