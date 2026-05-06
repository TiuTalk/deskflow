/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2025 Deskflow Developers
 * SPDX-FileCopyrightText: (C) 2012 - 2016 Symless Ltd.
 * SPDX-FileCopyrightText: (C) 2003 Chris Schoeneman
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#pragma once

#include <cstdint>

//! Screen interface
/*!
This interface defines the methods common to all screens.
*/
class IScreen
{
public:
  virtual ~IScreen() = default;

  //! @name accessors
  //@{

  //! Get event target
  /*!
  Returns the target used for events created by this object.
  */
  virtual void *getEventTarget() const = 0;

  //! Get screen shape
  /*!
  Return the position of the upper-left corner of the screen in \c x and
  \c y and the size of the screen in \c width and \c height.
  */
  virtual void getShape(int32_t &x, int32_t &y, int32_t &width, int32_t &height) const = 0;

  //! Get cursor position
  /*!
  Return the current position of the cursor in \c x and \c y.
  */
  virtual void getCursorPos(int32_t &x, int32_t &y) const = 0;

  //@}
};
