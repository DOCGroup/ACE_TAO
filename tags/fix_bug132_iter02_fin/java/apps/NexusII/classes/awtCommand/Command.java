/*
 * Copyright 1996 Jan Newmarch, University of Canberra.
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The author
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */



package awtCommand;

import java.awt.*;

public interface Command {

    /**
     * Executes application logic.
     * Called by events occurring in associated objects
     */
    public abstract void execute(Object target, Event evt, Object what); 
}
