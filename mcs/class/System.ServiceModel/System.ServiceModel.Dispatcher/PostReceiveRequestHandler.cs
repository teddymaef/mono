﻿using System;
using System.Collections.Generic;
using System.Text;
using System.ServiceModel.Channels;
using System.ServiceModel;

namespace System.ServiceModel.Dispatcher
{
	internal class PostReceiveRequestHandler : BaseRequestProcessorHandler
	{
		private static readonly object open_lock = new object ();

		protected override bool ProcessRequest (MessageProcessingContext mrc)
		{
			EnsureInstanceContextOpen (mrc.InstanceContext);
			AfterReceiveRequest (mrc);
			return false;
		}

		void AfterReceiveRequest (MessageProcessingContext mrc)
		{
			mrc.EventsHandler.AfterReceiveRequest ();
		}

		void EnsureInstanceContextOpen (InstanceContext ictx)
		{
			if (ictx.State != CommunicationState.Opened) {
				lock (open_lock) {
					if (ictx.State != CommunicationState.Opened)
						ictx.Open ();
				}
			}
		}
	}
}
