![http://blog.blogwhy.com/edge2d/upload/edge_full.jpg](http://blog.blogwhy.com/edge2d/upload/edge_full.jpg)
# Edge2d Game Engine #
**Edge2d version 0.1.0**
**Copyright © 2007-2008, Kevin Lynx. All rights reserved.**

## Overview ##

> Edge2d is a very object-oriented and plug-in-based 2d game engine. As a result of strict software design edge2d will be an easy extended, cross-platform and independent from any other 3rd libraries engine.

> Edge2d is absolutely plug-in-based. What plug-in-based here mean is that all the modules except the basic module of edge2d (also called abstract level) is plug-in enabled. For instance, you can develop your own graphics module to be an Edge2d plug-in and register it into Edge, and then Edge will work with your own graphics plug-in.

> The original developer of Edge recommends users to develop their own plug-ins. That’s the main design purpose of Edge. That means you can develop your own plug-ins easily but not modify the Edge’s basic module.

> Edge2d is open-source. The original developer tries his hard to show his experience on OOP to every engine user.

> I don’t want anything, I only want to share my little experience on programming especially on Object-oriented programming. I hope you will enjoy this library from my deep bottom of hear.

## Features ##
  * Strict focus on 2d games, even if the graphics plug-in uses 3D low level API, you don’t need to know any 3D information.
  * The core of Edge is designed to function-based, so it’s very flexible for you. But the helper library also provides an easy framework so that you can use the engine easily. What I really want is to provide convenient for you besides flexible.
  * The engine is easily extended by its plug-in design. I will provide enough documents to teach you how to program yourself plug-ins.
  * Edge2d is absolutely free.
  * The graphics plug-in recommend the plug-in developer to use 3D low level API like Direct3D and OpenGL to develop graphics module. That means Edge2d will use 3D hardware to accelerate its rendering.

## Architecture ##

> The release version of Edge2d includes three parts: Edge base, Edge helper and any kinds of plug-ins.

> The Edge base (also called abstract level) is mainly to provide your interface to use and some other basic functions like IO, log system and exceptions.

> The Edge helper includes some static library that has an easy framework, frame animation, resource management from xml and some other libraries taken from other library( like particle system in HGE)

> The plug-ins are many dynamic libraries in fact. These plug-ins implement the basis of Edge like Graphics, Image Codec, Input System, Sound System, IO reading from any kinds of compressed data files, Font, Operation System dependent modules etc.

![http://blog.blogwhy.com/edge2d/upload/ar.jpg](http://blog.blogwhy.com/edge2d/upload/ar.jpg)

## Downloads ##
### Edge VS2005 Binary ###
http://edge2d.googlecode.com/files/Edge_vc8_bin.zip

### Edge Documents ###
http://edge2d.googlecode.com/files/EdgeDoc.zip

### Edge Examples(bin and source code) ###
http://edge2d.googlecode.com/files/EdgeExamples_Full.zip

### Edge Full Game Demo : Crazy Eggs Clone ###
http://edge2d.googlecode.com/files/EdgeExample-CrazyEggsEdge_v3.zip

### Edge Full Game Demo : Popo Clone ###
http://edge2d.googlecode.com/files/EdgeExample_Popo_Full.zip


### Contact the original developer (Kevin Lynx) ###
[zmhn320@163.com]
