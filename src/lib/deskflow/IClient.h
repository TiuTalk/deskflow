/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2012 - 2016 Symless Ltd.
 * SPDX-FileCopyrightText: (C) 2002 Chris Schoeneman
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#pragma once

#include "deskflow/IScreen.h"
#include "deskflow/KeyTypes.h"
#include "deskflow/MouseTypes.h"
#include "deskflow/OptionTypes.h"

#include <string>

//! Client interface
/*!
This interface defines the methods necessary for the server to
communicate with a client.
*/
class IClient : public IScreen
{
public:
  //! @name manipulators
  //@{

  //! Enter screen
  /*!
  Enter the screen.  The cursor should be warped to \p xAbs,yAbs.
  \p mask is the expected toggle button state and the client should
  update its state to match.  \p forScreensaver is true iff the
  screen is being entered because the screen saver is starting.
  */
  virtual void enter(int32_t xAbs, int32_t yAbs, uint32_t seqNum, KeyModifierMask mask, bool forScreensaver) = 0;

  //! Leave screen
  /*!
  Leave the screen.  Return false iff the user may not leave the
  client's screen (because, for example, a button is down).
  */
  virtual bool leave() = 0;

  //! Notify of key press
  /*!
  Synthesize key events to generate a press of key \c id.  If possible
  match the given modifier mask.  The KeyButton identifies the physical
  key on the server that generated this key down.  The client must
  ensure that a key up or key repeat that uses the same KeyButton will
  synthesize an up or repeat for the same client key synthesized by
  keyDown().
  */
  virtual void keyDown(KeyID id, KeyModifierMask, KeyButton, const std::string &) = 0;

  //! Notify of key repeat
  /*!
  Synthesize key events to generate a press and release of key \c id
  \c count times.  If possible match the given modifier mask.
  */
  virtual void keyRepeat(KeyID id, KeyModifierMask, int32_t count, KeyButton, const std::string &lang) = 0;

  //! Notify of key release
  /*!
  Synthesize key events to generate a release of key \c id.  If possible
  match the given modifier mask.
  */
  virtual void keyUp(KeyID id, KeyModifierMask, KeyButton) = 0;

  //! Notify of mouse press
  /*!
  Synthesize mouse events to generate a press of mouse button \c id.
  */
  virtual void mouseDown(ButtonID id) = 0;

  //! Notify of mouse release
  /*!
  Synthesize mouse events to generate a release of mouse button \c id.
  */
  virtual void mouseUp(ButtonID id) = 0;

  //! Notify of mouse motion
  /*!
  Synthesize mouse events to generate mouse motion to the absolute
  screen position \c xAbs,yAbs.
  */
  virtual void mouseMove(int32_t xAbs, int32_t yAbs) = 0;

  //! Notify of mouse motion
  /*!
  Synthesize mouse events to generate mouse motion by the relative
  amount \c xRel,yRel.
  */
  virtual void mouseRelativeMove(int32_t xRel, int32_t yRel) = 0;

  //! Notify of mouse wheel motion
  /*!
  Synthesize mouse events to generate mouse wheel motion of \c xDelta
  and \c yDelta.  Deltas are positive for motion away from the user or
  to the right and negative for motion towards the user or to the left.
  Each wheel click should generate a delta of +/-120.
  */
  virtual void mouseWheel(int32_t xDelta, int32_t yDelta) = 0;

  //! Notify of screen saver change
  virtual void screensaver(bool activate) = 0;

  //! Notify of options changes
  /*!
  Reset all options to their default values.
  */
  virtual void resetOptions() = 0;

  //! Notify of options changes
  /*!
  Set options to given values.  Ignore unknown options and don't
  modify our options that aren't given in \c options.
  */
  virtual void setOptions(const OptionsList &options) = 0;

  //@}
  //! @name accessors
  //@{

  //! Get client name
  /*!
  Return the client's name.
  */
  virtual std::string getName() const = 0;

  //@}
};
