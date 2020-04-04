#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/// Called after hardware setup and before applicationLoop
void applicationSetup();

/// This is the main loop of application. Implements infinite loop, never returns.
void applicationLoop();
  
#ifdef __cplusplus
}
#endif