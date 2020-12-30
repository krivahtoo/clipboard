/*
 * Copyright (c) 2020 Noah Too
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


#include "cxxopts.hpp"
#include "clip/clip.h"


#include <iostream>
#include <fstream>


#define VERSION "O.1.0"

#ifdef _WIN32
  #define ERR_MSG "Error: "
#else
  #define ERR_MSG "\033[31mError:\033[0m "
#endif // #ifdef _WIN32

int main(int argc, char** argv)
{
    cxxopts::Options options("clipboard", "A CLI tool to copy/paste to/from clipboard/files");

    options.add_options()
        ("i,input", "Input file (clipboard if not specified)", cxxopts::value<std::string>(), "<file>")
        ("o,output", "Output file (clipboard if not specified)", cxxopts::value<std::string>(), "<file>")
        ("h,help", "Print usage")
        ("v,version", "Print version")
    ;
    
    options.custom_help("[OPTIONS].. <file>");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      exit(0);
    }
    if (result.count("version")) {
      std::cout << "Version " << VERSION << std::endl;
      exit(0);
    }

    std::string input, output, data;
    if (result.count("input"))
      input = result["input"].as<std::string>();
    if (result.count("output"))
      output = result["output"].as<std::string>();

    if (input.empty() && output.empty()) {
      std::cout << ERR_MSG << "You must specify either input and output file." << std::endl;
      exit(1);
    }

    if (input.empty()) {
      if (clip::has(clip::text_format())) {
        clip::get_text(data);
      } else {
        std::cout << ERR_MSG << "Clipboard doesn't contain text" << std::endl;
        exit(1);
      }
    } else {
      std::ifstream f(input);
      if (!f.is_open()) {
        // The file hasn't been opened; take appropriate actions here.
        std::cout << ERR_MSG << "File could not be opened" << std::endl;
        exit(1);
      }

      f.seekg(0, std::ios::end);
      data.reserve(f.tellg());
      f.seekg(0, std::ios::beg);
      data.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
      if (f.fail()) {
        std::cout << ERR_MSG << "Could not copy file contents" << std::endl;
        exit(1);
      }
    }

    if (output.empty()) {
      std::string out = data;
      if (clip::set_text(out)) {
        std::cout << "File copied to clipboard" << std::endl;
      } else {
        std::cout << ERR_MSG << "Could not copy to clipboard" << std::endl;
      }
    } else {
      std::ofstream os(output);
      if(os.is_open()){
        os << data;
        if (os.bad()) {
          std::cout << ERR_MSG << "Could not write to file" << std::endl;
          exit(1);
        }
      }
    }

    return 0;
}
