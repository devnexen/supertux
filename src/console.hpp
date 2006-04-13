//  $Id$
//
//  SuperTux - Console
//  Copyright (C) 2006 Christoph Sommer <christoph.sommer@2006.expires.deltadevelopment.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef SUPERTUX_CONSOLE_H
#define SUPERTUX_CONSOLE_H

#include <list>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <squirrel.h>

class Console;
class ConsoleStreamBuffer;
class ConsoleCommandReceiver;
class DrawingContext;
class Surface;
class Font;

class Console 
{
public:
  Console();
  ~Console();

  static Console* instance;

  static std::ostream input; /**< stream of keyboard input to send to the console. Do not forget to send std::endl or to flush the stream. */
  static std::ostream output; /**< stream of characters to output to the console. Do not forget to send std::endl or to flush the stream. */

  void backspace(); /**< delete last character sent to the input stream */
  void scroll(int offset); /**< scroll console text up or down by @c offset lines */
  void autocomplete(); /**< autocomplete current command */

  void draw(DrawingContext& context); /**< draw the console in a DrawingContext */
  void update(float elapsed_time);
  
  void show(); /**< display the console */
  void hide(); /**< hide the console */
  void toggle(); /**< display the console if hidden, hide otherwise */

  bool hasFocus(); /**< true if characters should be sent to the console instead of their normal target */
  void registerCommand(std::string command, ConsoleCommandReceiver* ccr); /**< associate command with the given CCR */
  void unregisterCommand(std::string command, ConsoleCommandReceiver* ccr); /**< dissociate command and CCR */
  void unregisterCommands(ConsoleCommandReceiver* ccr); /**< dissociate all commands of given CCR */

  template<typename T> static bool string_is(std::string s) {
    std::istringstream iss(s);
    T i;
    if ((iss >> i) && iss.eof()) {
      return true;
    } else {
      return false;
    }
  }

  template<typename T> static T string_to(std::string s) {
    std::istringstream iss(s);
    T i;
    if ((iss >> i) && iss.eof()) {
      return i;
    } else {
      return T();
    }
  }

private:
  std::list<std::string> lines; /**< backbuffer of lines sent to the console */
  std::map<std::string, std::list<ConsoleCommandReceiver*> > commands; /**< map of console commands and a list of associated ConsoleCommandReceivers */
  
  std::auto_ptr<Surface> background; /**< console background image */
  std::auto_ptr<Surface> background2; /**< second, moving console background image */

  HSQUIRRELVM vm; /**< squirrel thread for the console (with custom roottable */
  HSQOBJECT vm_object;
  
  int backgroundOffset; /**< current offset of scrolling background image */
  float height; /**< height of the console in px */
  float alpha;
  int offset; /**< decrease to scroll text up */
  bool focused; /**< true if console has input focus */
  std::auto_ptr<Font> font;

  float stayOpen;

  static ConsoleStreamBuffer inputBuffer; /**< stream buffer used by input stream */
  static ConsoleStreamBuffer outputBuffer; /**< stream buffer used by output stream */

  void addLine(std::string s); /**< display a line in the console */
  void parse(std::string s); /**< react to a given command */
    
  /** execute squirrel script and output result */
  void execute_script(const std::string& s);
    
  bool consoleCommand(std::string command, std::vector<std::string> arguments); /**< process internal command; return false if command was unknown, true otherwise */

  friend class ConsoleStreamBuffer;
  void flush(ConsoleStreamBuffer* buffer); /**< act upon changes in a ConsoleStreamBuffer */
};

class ConsoleStreamBuffer : public std::stringbuf 
{
  public:
    int sync() 
    {
      int result = std::stringbuf::sync();
      if(Console::instance != NULL)
        Console::instance->flush(this);
      return result;
    }
};

class ConsoleCommandReceiver
{
public:
  virtual ~ConsoleCommandReceiver()
  {
    Console::instance->unregisterCommands(this);
  }
   
  /**
   * callback from Console; return false if command was unknown,
   * true otherwise
   */
  virtual bool consoleCommand(std::string command, std::vector<std::string> arguments) = 0;
};

#endif
